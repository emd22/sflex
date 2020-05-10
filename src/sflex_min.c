#include "sflex.h"

#include <string.h>
#include <stdlib.h>

#define TOKEN_BUFFER_START 64

#ifndef bool
typedef enum { false, true } bool;
#endif

token_t *tokens = NULL;
static int tokens_buffer_size = TOKEN_BUFFER_START;
static int token_i = 0;

static bool is_whitespace(unsigned char ch) {
    switch (ch) {
        case ' ':
        case '\t':
        case '\n':
            return true;
    }
    return false;
}

static bool is_string(unsigned char ch) {
    switch (ch) {
        case '"':
        case '\'':
            return true;
    }
    return false;
}

token_t *token_get_next(void) {
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
        if (is_string(ch))
            in_string = !in_string;
            
        if ((is_whitespace(ch) || newb-1 == data) && (!in_string)) {
            if (token != NULL)
                token->end = newb-1;
            while (is_whitespace(*newb))
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
                if (!is_whitespace(lastch) && (token != NULL)) {
                    token->end = newb-1;
                    token = token_get_next();
                }
                token->start = newb-1;
                token->end = newb;
                token = token_get_next();
                while (is_whitespace(*(newb))) newb++;
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
