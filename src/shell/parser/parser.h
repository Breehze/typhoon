/**
 * @file parser.h
 * @brief Recursive descent parser for shell commands
 *
 * Builds an Abstract Syntax Tree (AST) from tokenized input.
 * Supports command lists (;), pipes (|), and I/O redirection (< >).
 */

#pragma once

#include "types.h"

/**
 * Parses a single command with optional I/O redirection.
 *
 * @param token_stream Pointer to current position in token array
 * @return AST node representing the command
 */
ASTnode * parse_command(char *** token_stream);

/**
 * Parses a list of commands separated by semicolons.
 * Entry point for parsing a complete command line.
 *
 * @param token_stream Pointer to current position in token array
 * @return AST node representing the command list
 */
ASTnode * parse_list(char *** token_stream);

/**
 * Frees all memory associated with an AST.
 *
 * @param root Root node of the AST to free
 */
void freeAST(ASTnode * root);

/**
 * Prints an AST for debugging purposes.
 *
 * @param node   Root node to print
 * @param indent Current indentation level
 */
void print_ast(ASTnode *node, int indent);
