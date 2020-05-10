#ifndef SFLEXH_H
#define SFLEXH_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define sflex_len(token) (token.end-token.start)
#define sflex_tok(token) (token.start)

typedef struct {
    char *start;
    char *end;
} token_t;

#define TOKEN_BUFFER_START 64

token_t *tokens = NULL;
static int tokens_buffer_size = TOKEN_BUFFER_START;
static int token_i = 0;

#define IS_WHITESPACE(ch) (ch == ' ' || ch == '\t' || ch == '\n')
#define IS_STRING(ch) (ch == '"' || ch == '\'')

static token_t *token_get_next(void) {
    if (token_i+1 > tokens_buffer_size) {
        tokens_buffer_size *= 2;
        tokens = realloc(tokens, tokens_buffer_size);
    }
    return &tokens[token_i++];
}

token_t *sflex(char *data, const char *specials, int *tokens_amt) {
    char ch, lastch;
    
    tokens = malloc(sizeof(token_t)*TOKEN_BUFFER_START);

    bool in_string = false;
    char *newb = data;
    token_t *token = NULL;
    while (ch = *(newb++)) {
        if (IS_STRING(ch))
            in_string = !in_string;
            
        if ((IS_WHITESPACE(ch) || newb-1 == data) && (!in_string)) {
            if (token != NULL)
                token->end = newb-1;
            while (IS_WHITESPACE(*newb))
                newb++;
            token = token_get_next();
            if (newb-1 == data)
                token->start = newb-1;
            else
                token->start = newb;
        } 
        else {
            char *res = strchr(specials, ch);
            if (res != NULL) {
                if (!IS_WHITESPACE(lastch) && (token != NULL)) {
                    token->end = newb-1;
                    token = token_get_next();
                }
                token->start = newb-1;
                token->end = newb;
                token = token_get_next();
                while (IS_WHITESPACE(*(newb))) newb++;
                token->start = newb;
            }
        }
        lastch = ch;
    }
    
    if (tokens[token_i-1].start[0] == 0)
        token_i--;
    
    (*tokens_amt) = token_i;
    return tokens;
}

#endif
