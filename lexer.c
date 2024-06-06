#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

Token *create_token(TokenType type, const char *value) {
    Token *token = malloc(sizeof(Token));
    if (!token) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->value = strdup(value);
    if (!token->value) {
        perror("strdup");
        free(token);
        exit(EXIT_FAILURE);
    }
    return token;
}

void free_token(Token *token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

Token **lexer(const char *input, int *num_tokens) {
    int capacity = 10;
    Token **tokens = malloc(capacity * sizeof(Token *));
    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int length = strlen(input);
    int i = 0, token_count = 0;
    while (i < length) {
        if (token_count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token *));
            if (!tokens) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        if (isspace(input[i])) {
            tokens[token_count++] = create_token(TOKEN_SPACE, " ");
            while (isspace(input[i])) i++;
        } else if (input[i] == '|') {
            tokens[token_count++] = create_token(TOKEN_PIPE, "|");
            i++;
        } else if (input[i] == '(' || input[i] == ')') {
            char bracket[2] = {input[i], '\0'};
            tokens[token_count++] = create_token(TOKEN_BRACKET, bracket);
            i++;
        } else if (input[i] == '&' && input[i+1] == '&') {
            tokens[token_count++] = create_token(TOKEN_AND, "&&");
            i += 2;
        } else if (input[i] == '|' && input[i+1] == '|') {
            tokens[token_count++] = create_token(TOKEN_OR, "||");
            i += 2;
        } else {
            int start = i;
            while (i < length && !isspace(input[i]) && input[i] != '|' &&
                   input[i] != '(' && input[i] != ')' && input[i] != '&') {
                i++;
            }
            char *substring = strndup(&input[start], i - start);
            if (!substring) {
                perror("strndup");
                exit(EXIT_FAILURE);
            }
            tokens[token_count++] = create_token(TOKEN_STRING, substring);
            free(substring);
        }
    }

    *num_tokens = token_count;
    return tokens;
}

void free_tokens(Token **tokens, int num_tokens) {
    for (int i = 0; i < num_tokens; i++) {
        free_token(tokens[i]);
    }
    free(tokens);
}
