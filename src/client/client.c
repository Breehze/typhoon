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
   

void run_client(char * ip_addr,short int port,char * unix_socket_path){
    int serv_socket = -1;
    terminal_setup();
    char out_buffer[1024] = {0};
    char in_buffer[1024] = {0};
    
    FileDescriptorSet fdset;
    fdset_init(&fdset);
    
    if(unix_socket_path){    
        serv_socket = connect_UDS(unix_socket_path);
    }else{
        serv_socket = connect_TCP(ip_addr,port);
    }
    
    if(serv_socket < 3){
        printf("[ERR] Could not connect to server!\n");
        fdset_clean(&fdset);
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
