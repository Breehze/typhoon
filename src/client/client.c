#include <sys/poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
 
#include "../terminal/terminal.h"
#include "../networking/fdset/fdset.h"
#include "../networking/utils/utils.h"
   

int main(void){
    terminal_setup();
    char out_buffer[1024] = {0};
    char in_buffer[1024] = {0};
    
    FileDescriptorSet fdset;
    fdset_init(&fdset);
    
    int serv_socket = connect_TCP("127.0.0.1",8080);
    if(serv_socket < 3){
        printf("[ERR] Could not connect to server!\n");
        exit(1);
    }
    fdset_add(&fdset,0,POLLIN);
    fdset_add(&fdset,serv_socket,POLLIN);
    while(0xB00BA){
        poll(fdset.descriptors,fdset.len,-1);
        for(int i = 0;i < fdset.len;i++){
            int fd = fdset.descriptors[i].fd;
            int revents = fdset.descriptors[i].revents; 
            
            if(fd == 0 && revents & POLLIN){
                int n = read(fd,in_buffer,sizeof(in_buffer)); 
                write(serv_socket,in_buffer,n);
            }else if(fd == serv_socket && revents & POLLIN){
                int n = read(fd,out_buffer,sizeof(out_buffer));
                if(!n){
                    exit(0);
                }
                write(STDOUT_FILENO,out_buffer,n);
            }
        }
    }
}
