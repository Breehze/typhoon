#pragma once

#include <sys/types.h>

typedef enum err{
    NO_ERR,
    SOME_ERROR,
}Error;

typedef struct{
    pid_t pid;
    int master_fd;
}ChildProcessInfo;


void executor_exec(char *string,int fd, Error * err);
