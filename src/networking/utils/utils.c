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
