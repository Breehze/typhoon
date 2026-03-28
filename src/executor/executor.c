#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pty.h>

#include "executor.h"

char ** tokenize(char * string){
    int token_amount = 10;
    int filled = 0;
    char ** tokenized = (char **)malloc(token_amount * sizeof(char *));
    
    char * token = strtok(string," \n\r\t");

    while(token){
        if(filled + 1 > token_amount){
            token_amount *= 2 ;
            char ** new_tokenized = (char **)realloc(tokenized,token_amount * sizeof(char *));
            assert(new_tokenized);
            tokenized = new_tokenized;
        }

        tokenized[filled++] = strdup(token);
        token = strtok(NULL," \n\r\t");
    }
    tokenized[filled] = NULL;
    return tokenized;
}


void run_process(char ** tokenized_bin,Error *err){
    pid_t pid = fork();
    int status;

    if(pid == -1) { return; }
    
    if(pid == 0){
        if(execvp(tokenized_bin[0],tokenized_bin) == -1 && err){
            *err = SOME_ERROR;
        }; 
        _exit(EXIT_FAILURE);
    }
    waitpid(pid,NULL,0);
}

void executor_exec(char *string,int fd, Error * err){
    char ** tokens = tokenize(string);
    int child_pid;
    return run_process(tokens,NULL);
}
