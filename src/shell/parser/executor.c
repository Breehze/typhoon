#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <pty.h>

#include "lexer.h"
#include "types.h"
#include "parser.h"
#include "../../terminal/terminal.h"

void execute_pipe(ASTnode* node);


void run_process(ASTnode * node,Error *err){
    int master_fd;
    pid_t pid = fork();
    int status;

    if(pid == -1) { return; }
    
    if(pid == 0){
        if (node->input_file) {
            int fd = open(node->input_file, O_RDONLY );
            if (fd < 0) { _exit(EXIT_FAILURE); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (node->output_file) {
            int fd = open(node->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {  _exit(EXIT_FAILURE); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(execvp(node->command[0],node->command) == -1 && err){
            *err = SOME_ERROR;
        }; 
        _exit(EXIT_FAILURE);
    }
    waitpid(pid,NULL,0);
}

void execute_commnad(ASTnode * cmd_node,Error *err){
    if(!cmd_node->command || *cmd_node->command == NULL){
        if(err){ *err = SOME_ERROR; }
        return;
    }
    if(strcmp(*cmd_node->command,"help") == 0){
        print_usage("typhoon");
        return;
    }else if(strcmp(*cmd_node->command,"quit") == 0){
        exit(0);
        return;
    }else if(strcmp(*cmd_node->command,"halt") == 0){
        pid_t parent = getppid();
        if(parent == 1){ return; }
        kill(parent,SIGINT);
        return;
    }else if(strcmp(*cmd_node->command,"cd") == 0){
        if(cmd_node->command[1] == NULL){
            return;
        }
        chdir(cmd_node->command[1]);
        return;
    }

    run_process(cmd_node, err);
}

void execute_ast(ASTnode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case LIST:
            execute_ast(node->left);
            //wait(NULL);
            execute_ast(node->right);
            break;

        case PIPE:
            execute_pipe(node); 
            break;

        case COMMAND:
            execute_commnad(node,NULL);
            break;
    }
}

void execute_pipe(ASTnode *node) {
    int pfd[2];
    pipe(pfd); 

    if (fork() == 0) {         
        dup2(pfd[1], 1);       
        close(pfd[0]);         
        close(pfd[1]);         
        execute_ast(node->left);
        _exit(0);
    }

    if (fork() == 0) {         
        dup2(pfd[0], 0);       
        close(pfd[0]);         
        close(pfd[1]);         
        execute_ast(node->right);
        _exit(0);
    }

    close(pfd[0]); close(pfd[1]); 
    wait(NULL); wait(NULL);       

}


 void executor_exec(char *string, Error * err){
    char ** tokens_original = tokenize(string);
    char ** tokens = tokens_original;
    
    ASTnode * parsed = parse_list(&tokens); 
    
    execute_ast(parsed);
    
    freeAST(parsed);
    
    for(int i = 0; tokens_original[i] != NULL; i++){
        free(tokens_original[i]);
    }
    
    free(tokens_original);
}
