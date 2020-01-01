

#include <string.h>
#include <stdio.h>

#define TOKLEN(token) (token.end-token.start)
#define IS_WHITESPACE(ch) (ch == ' ' || ch == '\t' || ch == '\n')

typedef struct {
    char *start;
    char *end;
} token_t;

token_t tokens[256];
int token_i = 0;

void lex(char *data, const char *specials) {
    char ch, lastch;
    char in_string = 0;
    char *newb = data;
    token_t *token = NULL;
    
    // crawl through buffer until we hit 0
    while (ch = *(newb++)) {
        // if single quotes or double quotes, toggle string
        if (ch == '"' || ch == '\'')
            in_string = !in_string;
        // current character is whitespace
        if (IS_WHITESPACE(ch) && (!in_string)) {
            // don't set the end of the last token if we are on the first one
            if (token != NULL)
                token->end = newb-1;
            // skip until there are no whitespace left
            while (IS_WHITESPACE(*newb)) 
                newb++;
            // give us a fresh token
            token = &tokens[token_i++];
            // set the start position
            token->start = newb;
        } 
        else {
            // check if our current char is in the 'specials' array
            char *res = strchr(specials, ch);
            // it is
            if (res != NULL) {
                // if last character was whitespace, and we aren't on the first token,
                if (!IS_WHITESPACE(lastch) && (token != NULL)) {
                    // set end of token
                    token->end = newb-1;
                    // get a new token
                    token = &tokens[token_i++];
                }
                // setup token
                token->start = newb-1;
                token->end = newb;
                
                token = &tokens[token_i++];
                // if there is whitespace following our special character,
                // skip until valid character
                while (IS_WHITESPACE(*(newb)))
                    newb++;
                token->start = newb;
            }
        }
        lastch = ch;
    }
    // if we have an empty token at the end(usually whitespace) chop it off
    if (tokens[token_i-1].start[0] == 0)
        token_i--;
}

int main() {
    char *data = "   This   is\n\t a+test   'of the' single   *function   lexer!  \n";
    lex(data, "+*!");
    int i;
    for (i = 0; i < token_i; i++) {
        printf("TOK[%.*s]\n", TOKLEN(tokens[i]), tokens[i].start);
    }
    return 0;
}

