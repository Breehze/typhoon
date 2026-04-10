#include <signal.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <pty.h>
#include <termios.h>
#include <stdio.h>
#include <signal.h>

#include "../shell/typhoon.h"
#include "../shell/parser/types.h"

#include "../terminal/terminal.h"

#include "../networking/utils/utils.h"
#include "../networking/connection/connection.h"
#include "../networking/fdset/fdset.h"

#define FD_MAP_SIZE 4096

Connection * fd_to_conn[FD_MAP_SIZE] = {0};
volatile sig_atomic_t quit_sig = 0;

ChildProcessInfo spawnPTY(Error *err){
    int master_fd;
    pid_t pid = forkpty(&master_fd,NULL,NULL,NULL);
    int status;

    if(pid == -1) { return (ChildProcessInfo){.pid = pid,.master_fd = -1}; }

    if(pid == 0){
        setenv("TERM", "xterm-256color", 1);
        terminal_setup();
        run_shell();
        exit(0);
    }
    return (ChildProcessInfo){.pid = pid,.master_fd = master_fd};
}

void quit(int sig){
   quit_sig = 1;
}

void cleanup_disconnects(FileDescriptorSet * fdset){
    for(int i = fdset->len - 1; i >= 0;i--){
        Connection * conn = fd_to_conn[fdset->descriptors[i].fd];
        int fd = fdset->descriptors[i].fd;
        if(conn && conn->remove){
            fd_to_conn[fd] = NULL;
    
            int other_fd = (conn->socket_fd == fd ) ?  conn->pty_fd : conn->socket_fd ;
    
            fdset_remove(fdset,fd);
    
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

void cleanup(FileDescriptorSet * fdset){
    for (int i = 0; i < FD_MAP_SIZE; i++) {
        if (fd_to_conn[i]) {
            fd_to_conn[i]->remove = 1;
        }
    }
    cleanup_disconnects(fdset);
    fdset_clean(fdset);
    exit(0);
}


int run_server(char * ip_addr, short int port, char * unix_socket_path) {
    signal(SIGINT,quit);
    terminal_setup();
    int server_fd;
    if((!ip_addr || port == -1) && !unix_socket_path){
        return 1;
    }
    
    if(unix_socket_path){
        server_fd = server_listen_UDS(unix_socket_path);
    }else{
        server_fd = server_listen(port);
    }
        

    FileDescriptorSet fdset;
    fdset_init(&fdset);

    fdset_add(&fdset,server_fd,POLLIN);

    Connection * server_conn = spawnConnection(0);
    ChildProcessInfo server_pty = spawnPTY(NULL);
    server_conn->pty_fd = server_pty.master_fd;
    fdset_add(&fdset,0,POLLIN);
    fdset_add(&fdset,server_pty.master_fd,POLLIN);
    fd_to_conn[0] = server_conn;
    fd_to_conn[server_pty.master_fd] = server_conn;
    
    while(0xB00BA){
        poll(fdset.descriptors,fdset.len,500);
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

                fdset_add(&fdset,client_fd,POLLIN);
                fdset_add(&fdset,pty_info.master_fd,POLLIN);

                fd_to_conn[client_fd] = new_client;
                fd_to_conn[new_client->pty_fd] = new_client;

            } else if (event & (POLLIN | POLLHUP) ) {
                Connection * conn = fd_to_conn[fd];
                char * buffer = (fd == conn->socket_fd) ? conn->socket_buffer : conn->pty_buffer;
                int read_b = read(fd,buffer,1024);

                if(read_b <= 0){
                    conn->remove = 1;
                    printf("Someone disconnected\n");
                    continue;
                }

                if(fd == conn->pty_fd){
                    write(conn->socket_fd,conn->pty_buffer,read_b);
                }else if(fd == conn->socket_fd){
                    write(conn->pty_fd,conn->socket_buffer,read_b);
                }
            }
        }
        
        cleanup_disconnects(&fdset);
        
        if(quit_sig){
            cleanup(&fdset);
        }
    }
    return 0;
}
