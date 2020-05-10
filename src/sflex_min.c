#include "sflex.h"

#include <string.h>

#ifndef bool
typedef enum { false, true } bool;
#endif

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

int sflex(char *data, const char *specials, token_t *tokens) {
    char ch, lastch;
    
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
            token = &tokens[token_i++];
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
                    token = &tokens[token_i++];
                }
                token->start = newb-1;
                token->end = newb;
                token = &tokens[token_i++];
                while (is_whitespace(*(newb))) newb++;
                token->start = newb;
            }
        }
        lastch = ch;
    }
    
    if (tokens[token_i-1].start[0] == 0)
        token_i--;
    
    return token_i;
}
