#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include <netinet/in.h>


int server_listen(unsigned short port){
    struct sockaddr_in address;
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    int bind_result = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd,10);

    return server_fd;
}


int server_listen_UDS(const char *socket_path) {
        struct sockaddr_un address;
        int server_fd;
   
        server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        
   
        memset(&address, 0, sizeof(struct sockaddr_un));
        address.sun_family = AF_UNIX;
        
        strncpy(address.sun_path, socket_path, sizeof(address.sun_path) - 1);
   
        unlink(socket_path);
   
        int bind_result = bind(server_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un));
   
        listen(server_fd, 10);
               
        return server_fd;
    }
