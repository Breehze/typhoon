#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "types.h"
#include "parser.h"

void print_ast(ASTnode *node, int indent) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    switch (node->type) {
        case COMMAND:
            printf("COMMAND:");
            for (int i = 0; node->command[i] != NULL; i++) {
                printf(" %s", node->command[i]);
            }
            printf("\n");
            break;
        case PIPE:
            printf("PIPE\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case LIST:
            printf("LIST\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        default:
            printf("UNKNOWN NODE TYPE\n");
            break;
    }
}


char * peek(char ** token_stream){
    return *token_stream;
}


char * consume(char *** token_stream){
    return *(*token_stream)++;
}


ASTnode * spawn_higher_lvl_node(ASTnode * left,ASTnode * right, ASTnodeType type){
    if(!left || !right){ return NULL; }
    
    ASTnode * newNode = (ASTnode*)malloc(sizeof(ASTnode));
    
    if(!newNode) {return NULL;}
    
    *newNode = (ASTnode){
        .type = type,
        .left = left,
        .right = right
    };
    return newNode;
}


ASTnode * spawn_command_node(char ** command,char * input, char * output){
    if(!command) { return NULL;}
        
    ASTnode * newNode = (ASTnode*)malloc(sizeof(ASTnode));
    if(!newNode){ return NULL; }
    
    *newNode = (ASTnode){
        .type = COMMAND,
        .command = command,
        .input_file = input,
        .output_file = output
    };
    return newNode;
}


ASTnode * parse_command(char *** token_stream){
    char * input = NULL;
    char * output = NULL;
    char ** whole_command = (char **)malloc(20 * sizeof(char *));
    int i = 0;
    
    while(peek(*token_stream) != NULL && strcmp(peek(*token_stream),"|") != 0 && strcmp(peek(*token_stream),";") != 0){
        char * token = consume(token_stream);
        if(strcmp(token,">") == 0){
            if(!peek(*token_stream)){ free(whole_command) ; return NULL;}
            output = consume(token_stream);
        }else if(strcmp(token,"<") == 0){
            if(!peek(*token_stream)){ free(whole_command) ; return NULL;}
            input = consume(token_stream);
        }else{
            whole_command[i++] = token;
        }
        
    }
    whole_command[i] = NULL;

    return spawn_command_node(whole_command,input,output); 
}


ASTnode * parse_pipe(char *** token_stream){
    ASTnode * left = parse_command(token_stream);

    if(peek(*token_stream) != NULL && strcmp(peek(*token_stream),"|") == 0){
        consume(token_stream);
        ASTnode * right = parse_pipe(token_stream);
        return spawn_higher_lvl_node(left,right,PIPE);
    }
    return left;
}


ASTnode * parse_list(char *** token_stream){
    ASTnode * left = parse_pipe(token_stream);

    if(peek(*token_stream) != NULL && strcmp(peek(*token_stream),";") == 0){
        consume(token_stream);
        ASTnode * right = parse_list(token_stream);
        return spawn_higher_lvl_node(left,right,LIST);
    }
    return left;
}

void freeAST(ASTnode * root){
    if(!root) { return; }
    if(root->type == COMMAND){
        free(root->command);
        free(root);
        return;
    }
    freeAST(root->left);
    freeAST(root->right);
    free(root);
}
