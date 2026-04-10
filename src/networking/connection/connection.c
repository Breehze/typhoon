#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "connection.h"


Connection * spawnConnection(int socket_fd){
    Connection * newConnection = (Connection *)malloc(sizeof(Connection));
    *newConnection = (Connection){
        .socket_fd = socket_fd,
        .pty_fd = -1,
        .child_pid = -1,
        .remove = 0,
        .last_ping = time(NULL)

    };
    return newConnection;
}

void flagInactive(Connection ** connections,size_t arr_size,time_t delta){
    time_t snapshot_now = time(NULL);
    for(int i = 0;i < arr_size;i++){
        if(!connections[i]){ continue; }
        if(snapshot_now > connections[i]->last_ping + delta){
            printf("Removing a connection\n");
            connections[i]->remove = 1;
        }
    }
}

void updateActivity(Connection * connection){
    connection->last_ping = time(NULL);
}
