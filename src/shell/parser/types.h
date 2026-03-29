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

typedef enum{
    COMMAND,
    PIPE,
    LIST
}ASTnodeType;

typedef struct ASTnode{
    ASTnodeType type;
    union {
        char ** command; 
        struct {
            struct ASTnode *left;
            struct ASTnode *right;
        };
    };
}ASTnode;
