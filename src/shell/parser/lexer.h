/**
 * @file lexer.h
 * @brief Lexical analyzer for shell command input
 *
 * Provides tokenization of raw command strings into an array of tokens
 * for further processing by the parser.
 */

#pragma once

/**
 * Tokenizes a command string into an array of tokens.
 *
 * @param string The input command string to tokenize
 * @return NULL-terminated array of token strings (caller must free)
 */
char ** tokenize(char * string);
