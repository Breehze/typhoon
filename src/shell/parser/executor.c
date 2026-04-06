#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pty.h>

#include "lexer.h"
#include "types.h"
#include "parser.h"

void execute_pipe(ASTnode* node);


void run_process(char ** tokenized_bin,Error *err){
    int master_fd;
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

void execute_commnad(char ** tokenized_path,Error *err){
    if(!tokenized_path || *tokenized_path == NULL){
        if(err){ *err = SOME_ERROR; }
        return;
    }
    if(strcmp(*tokenized_path,"help") == 0){
        printf("This shell was implemented as a part of SPAASM FIIT Course.\nAuthor of this program is Boris Ftorek.\n");
        return;
    }else if(strcmp(*tokenized_path,"quit") == 0){
        exit(0);
        return;
    }else if(strcmp(*tokenized_path,"halt") == 0){
        return;
    }else if(strcmp(*tokenized_path,"cd") == 0){
        if(tokenized_path[1] == NULL){
            return;
        }
        chdir(tokenized_path[1]);
        return;
    }

    run_process(tokenized_path, err);
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
            execute_commnad(node->command,NULL);
            break;
    }
}

void execute_pipe(ASTnode *node) {
    int pfd[2];
    pipe(pfd); 

    if (fork() == 0) {         
        dup2(pfd[1], 1);       
        close(pfd[0]);         
        execute_ast(node->left);
        exit(0);
    }

    if (fork() == 0) {         
        dup2(pfd[0], 0);       
        close(pfd[1]);         
        execute_ast(node->right);
        exit(0);
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
