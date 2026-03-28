#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <pty.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "executor/executor.h"

#define FD_MAP_SIZE 4096

typedef struct{
    int socket_fd;
    int pty_fd;
    pid_t child_pid;
    char pty_buffer[1024];
    char socket_buffer[1024];
    int remove;
}Connection;

Connection * fd_to_conn[FD_MAP_SIZE] = {0};

Connection * spawnConnection(int socket_fd){
    Connection * newConnection = (Connection *)malloc(sizeof(Connection));
    *newConnection = (Connection){
        .socket_fd = socket_fd,
        .pty_fd = -1,
        .child_pid = -1,
        .remove = 0
    };
    return newConnection;
}


typedef struct{
    struct pollfd * descriptors;
    int len;
    int capacity;
}FileDescriptorSet;

void initFDSet(FileDescriptorSet * fdset){
    fdset->capacity = 10;
    fdset->len = 0;
    fdset->descriptors = (struct pollfd *)malloc(fdset->capacity* sizeof(struct pollfd));
    assert(fdset->descriptors);
}

void addFD(FileDescriptorSet * fdset,int fd,int mode){
    if(fdset->len + 1 > fdset->capacity){
        fdset->capacity *= 2;
        struct pollfd * new_fdset_mem = (struct pollfd *)realloc(fdset->descriptors,fdset->capacity * sizeof(struct pollfd));
        fdset->descriptors = new_fdset_mem;
    }
    fdset->descriptors[fdset->len] = (struct pollfd){
        .fd = fd,
        .events = mode 
    };
    fdset->len++;
}

void removeFD(FileDescriptorSet *fdset, int fd) {
    int i;
    for(i = 0;i<fdset->len;i++){
        if(fdset->descriptors[i].fd == fd) break;
    }
    close(fdset->descriptors[i].fd);
    fdset->descriptors[i] = fdset->descriptors[fdset->len - 1];
    fdset->len--;
}

void tearFDset(FileDescriptorSet * fdset){
    free(fdset->descriptors);
    fdset->capacity = 0;
    fdset->len = 0;
}

void run_shell(){
    char buffer[1024] = {0};
    int pos = 0;
    write(1, "boris@zephyr ~/ ", 16);
    while(0xB00BA){
        char c;
        int n = read(0, &c, 1);
        if(n <= 0) break;

        if(c == '\r' || c == '\n'){
            buffer[pos] = '\0';
            write(1, "\r\n", 2);
            if(pos > 0){
                executor_exec(buffer, -1, NULL);
            }
            pos = 0;
            memset(buffer, 0, sizeof(buffer));
            write(1, "boris@zephyr ~/ ", 16);
        } else if(c == 127 || c == '\b'){
            if(pos > 0){
                pos--;
                write(1, "\b \b", 3);
            }
        } else {
            buffer[pos++] = c;
            write(1, &c, 1);
        }
    }
}



ChildProcessInfo spawnPTY(Error *err){
    int master_fd;
    pid_t pid = forkpty(&master_fd,NULL,NULL,NULL);
    int status;

    if(pid == -1) { return (ChildProcessInfo){.pid = pid,.master_fd = -1}; }
    
    if(pid == 0){
         setenv("TERM", "xterm-256color", 1);
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag &= ~(ECHO | ICANON);  
        t.c_cc[VMIN] = 1;   
        t.c_cc[VTIME] = 0;  
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        run_shell();
        _exit(EXIT_FAILURE);
    }
    return (ChildProcessInfo){.pid = pid,.master_fd = master_fd};
}


int main() { 
    printf("What the fuck\n");
    int server_fd, client_fd;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    int bind_result = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 1);

    FileDescriptorSet fdset;
    initFDSet(&fdset);

    addFD(&fdset,server_fd,POLLIN);
    
    while(0xB00BA){
        poll(fdset.descriptors,fdset.len,-1);
        int fdset_len_snapshot = fdset.len;
        
        for(int i = 0; i < fdset_len_snapshot ;i++){
            int fd = fdset.descriptors[i].fd;
            short event = fdset.descriptors[i].revents;
            
            if(fd == server_fd && event & POLLIN){
                int client_fd = accept(server_fd,NULL,NULL);
                ChildProcessInfo pty_info = spawnPTY(NULL);

                Connection * new_client = spawnConnection(client_fd);
                new_client->pty_fd = pty_info.master_fd;
                new_client->child_pid = pty_info.pid;
                
                addFD(&fdset,client_fd,POLLIN);
                addFD(&fdset,pty_info.master_fd,POLLIN);
                
                fd_to_conn[client_fd] = new_client;
                fd_to_conn[new_client->pty_fd] = new_client;    
            
            } else if (event & POLLIN) {
                Connection * conn = fd_to_conn[fd];
                char * buffer = (fd == conn->socket_fd) ? conn->socket_buffer : conn->pty_buffer;
                int read_b = read(fd,buffer,1024);

                if(read_b <= 0){
                    conn->remove = 1;
                    continue;
                }

                if(fd == conn->pty_fd){
                    write(conn->socket_fd,conn->pty_buffer,read_b);
                }else if(fd == conn->socket_fd){
                    write(conn->pty_fd,conn->socket_buffer,read_b);
                }
            }
        }
        
        for(int i = fdset.len - 1; i >= 0;i--){
            Connection * conn = fd_to_conn[fdset.descriptors[i].fd];
            int fd = fdset.descriptors[i].fd;
            if(conn && conn->remove){
                fd_to_conn[fd] = NULL;

                int other_fd = (conn->socket_fd == fd ) ?  conn->pty_fd : conn->socket_fd ;

                removeFD(&fdset,fd);

                if(fd_to_conn[other_fd]){
                    continue;
                }

                if(conn->child_pid > 0){
                    kill(conn->child_pid, SIGTERM);
                    waitpid(conn->child_pid, NULL, WNOHANG);
                }

                free(conn);      
            }
        };       
    }
    
    return 0;
}

