#pragma once
#include <unistd.h>

typedef struct{
    int socket_fd;
    int pty_fd;
    pid_t child_pid;
    char pty_buffer[1024];
    char socket_buffer[1024];
    int remove;
}Connection;

Connection * spawnConnection(int socket_fd);
