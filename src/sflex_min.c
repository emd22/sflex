#include "sflex.h"
#include <string.h>

#define IS_WHITESPACE(ch) (ch == ' ' || ch == '\t' || ch == '\n')

int token_i = 0;

int sflex(char *data, const char *specials, token_t *tokens) {
    char ch, lastch, in_string = 0;
    char *newb = data;
    token_t *token = NULL;
    while (ch = *(newb++)) {
        if (ch == '"' || ch == '\'') in_string = !in_string;
        if (IS_WHITESPACE(ch) && (!in_string)) {
            if (token != NULL) token->end = newb-1;
            while (IS_WHITESPACE(*newb)) newb++;
            token = &tokens[token_i++];
            token->start = newb;
        } 
        else {
            char *res = strchr(specials, ch);
            if (res != NULL) {
                if (!IS_WHITESPACE(lastch) && (token != NULL)) {
                    token->end = newb-1;
                    token = &tokens[token_i++];
                }
                token->start = newb-1;
                token->end = newb;
                token = &tokens[token_i++];
                while (IS_WHITESPACE(*(newb))) newb++;
                token->start = newb;
            }
        }
        lastch = ch;
    }
    if (tokens[token_i-1].start[0] == 0) token_i--;
    return token_i;
}

