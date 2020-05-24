#ifndef SFLEXH_H
#define SFLEXH_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SFLEX_USE_STRINGS 0x01

#define sflex_len(token) (token.end-token.start)
#define sflex_tok(token) (token.start)

typedef struct {
    char *start;
    char *end;
} sflex_token_t;

typedef struct {
    sflex_token_t *tokens;
    int token_buffer_size;
    int token_amt;
} sflex_t;

#define TOKEN_BUFFER_START 64

#define SFLEX_IS_WHITESPACE(ch) (ch == ' ' || ch == '\t' || ch == '\n')
#define SFLEX_IS_STRING(ch, flags) ((flags & SFLEX_USE_STRINGS) && (ch == '"' || ch == '\''))
#define SFLEX_IS_SPECIAL(buf, ch) (strchr(buf, ch) != NULL)

static sflex_token_t *_sflex_token_get_next(sflex_t *inst) {
    if (inst->token_amt+1 > inst->token_buffer_size) {
        inst->token_buffer_size *= 2;
        inst->tokens = realloc(inst->tokens, inst->token_buffer_size);
    }
    return &inst->tokens[inst->token_amt++];
}

sflex_t sflex(char *data, const char *specials, int flags) {    
    char ch, lastch;
    // Setup sflex structure
    sflex_t inst;
    inst.token_buffer_size = TOKEN_BUFFER_START;
    inst.token_amt = 0;
    inst.tokens = malloc(sizeof(sflex_token_t)*inst.token_buffer_size);    

    bool in_string = false;
    
    char *newb = data;
    sflex_token_t *token = NULL;
    
    while (ch = *(newb)) {
        if (SFLEX_IS_STRING(ch, flags))
            in_string = !in_string;
        
        // handle whitespace
        if ((SFLEX_IS_WHITESPACE(ch) || newb == data) && (!in_string)) {
            if (token != NULL)
                token->end = newb;
            
            while (SFLEX_IS_WHITESPACE(*(newb+1)))
                newb++;
            
            token = _sflex_token_get_next(&inst);
            
            if (newb == data && !SFLEX_IS_WHITESPACE(ch))
                token->start = newb;
            else
                token->start = newb+1;
        }
        // handle 'special' characters
        else {
            if (specials != NULL && SFLEX_IS_SPECIAL(specials, ch) && !in_string) {
                if (!SFLEX_IS_WHITESPACE(lastch) && !SFLEX_IS_SPECIAL(specials, lastch) && (token != NULL)) {
                    token->end = newb;
                    token = _sflex_token_get_next(&inst);
                }
                token->start = newb;
                token->end = newb+1;
                token = _sflex_token_get_next(&inst);
                while (SFLEX_IS_WHITESPACE(*(newb+1)))
                    newb++;
                token->start = newb+1;
            }
        }
        lastch = ch;
        newb++;
    }
    
    // chop off empty token at end
    inst.token_amt--;
    return inst;
}

void sflex_destroy(sflex_t *inst) {
    if (inst == NULL)
        return;
        
    if (inst->tokens != NULL)
        free(inst->tokens);
    inst->token_amt = 0;
    inst->token_buffer_size = 0;
}

#endif
