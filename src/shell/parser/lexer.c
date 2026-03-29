#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
