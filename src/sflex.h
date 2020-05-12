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
#define IS_SPECIAL(buf, ch) (strchr(buf, ch) != NULL)

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
    
    while (ch = *(newb)) {
        if (IS_STRING(ch))
            in_string = !in_string;
        
        // handle whitespace
        if ((IS_WHITESPACE(ch) || newb == data) && (!in_string)) {
            if (token != NULL)
                token->end = newb;
            
            while (IS_WHITESPACE(*(newb+1)))
                newb++;
            
            token = token_get_next();
            
            if (newb == data && !IS_WHITESPACE(ch))
                token->start = newb;
            else
                token->start = newb+1;
        }
        // handle 'special' characters
        else {
            if (IS_SPECIAL(specials, ch) && !in_string) {
                if (!IS_WHITESPACE(lastch) && !IS_SPECIAL(specials, lastch) && (token != NULL)) {
                    token->end = newb;
                    token = token_get_next();
                }
                token->start = newb;
                token->end = newb+1;
                token = token_get_next();
                while (IS_WHITESPACE(*(newb+1)))
                    newb++;
                token->start = newb+1;
            }
        }
        lastch = ch;
        newb++;
    }
    
    token_i--;
    
    (*tokens_amt) = token_i;
    return tokens;
}

#endif
