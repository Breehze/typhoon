#include <stdlib.h>
#include <string.h>

#include "types.h"


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


ASTnode * spawn_command_node(char ** command){
    if(!command) { return NULL;}
        
    ASTnode * newNode = (ASTnode*)malloc(sizeof(ASTnode));
    if(!newNode){ return NULL; }
    
    *newNode = (ASTnode){
        .type = COMMAND,
        .command = command,
    };
    return newNode;
}


ASTnode * parse_command(char *** token_stream){
    char ** whole_command = (char **)malloc(64 * sizeof(char *));
    int i = 0;
    
    while(peek(*token_stream) != NULL && strcmp(peek(*token_stream),"|") != 0 && strcmp(peek(*token_stream),";") != 0){
        whole_command[i++] = consume(token_stream);
    }
    whole_command[i] = NULL;

    return spawn_command_node(whole_command); 
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
