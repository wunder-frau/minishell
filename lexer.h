#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_PIPE,
    TOKEN_BRACKET,
    TOKEN_COMMAND,
    TOKEN_REDIR,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_STRING,
    TOKEN_SPACE,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token *create_token(TokenType type, const char *value);
void free_token(Token *token);
Token **lexer(const char *input, int *num_tokens);
void free_tokens(Token **tokens, int num_tokens);

#endif
