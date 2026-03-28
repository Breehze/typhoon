#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pty.h>

#include "lexer.h"
#include "types.h"
#include "parser.h"

void execute_pipe(ASTnode*,int);

ChildProcessInfo run_process(char ** tokenized_bin,Error *err){
    int master_fd;
    pid_t pid = forkpty(&master_fd,NULL,NULL,NULL);
    int status;

    if(pid == -1) { return (ChildProcessInfo){.pid = pid,.master_fd = -1}; }
    
    if(pid == 0){
        if(execvp(tokenized_bin[0],tokenized_bin) == -1 && err){
            *err = SOME_ERROR;
        }; 
        _exit(EXIT_FAILURE);
    }
    return (ChildProcessInfo){.pid = pid,.master_fd = master_fd};
    
}


void execute_ast(ASTnode *node,int fd) {
    if (node == NULL) return;

    switch (node->type) {
        case LIST:
            execute_ast(node->left,fd);
            //wait(NULL);
            execute_ast(node->right,fd);
            break;

        case PIPE:
            execute_pipe(node,fd); 
            break;

        case COMMAND:
            run_process(node->command,NULL);
            break;
    }
}

void execute_pipe(ASTnode *node,int fd) {
    int pfd[2];
    pipe(pfd); 

    if (fork() == 0) {         
        dup2(pfd[1], 1);       
        close(pfd[0]);         
        execute_ast(node->left,fd);
        exit(0);
    }

    if (fork() == 0) {         
        dup2(pfd[0], 0);       
        close(pfd[1]);         
        execute_ast(node->right,fd);
        exit(0);
    }

    close(pfd[0]); close(pfd[1]); 
    wait(NULL); wait(NULL);       

}


ChildProcessInfo executor_exec(char *string,int fd, Error * err){
    char ** tokens = tokenize(string);
    int child_pid;
    return run_process(tokens,NULL);
    //ASTnode * parsed = parse_command(&tokens); 
    //execute_ast(parsed,fd);
}
