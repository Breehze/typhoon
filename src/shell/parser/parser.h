#pragma once

#include "types.h"

ASTnode * parse_command(char *** token_stream);
ASTnode * parse_list(char *** token_stream);


void freeAST(ASTnode * root);
void print_ast(ASTnode *node, int indent);
