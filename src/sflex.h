#ifndef SFLEX_H
#define SFLEX_H

#define sflex_len(token) (token.end-token.start)
#define sflex_tok(token) (token.start)

typedef struct {
    char *start;
    char *end;
} token_t;

token_t *sflex(char *data, const char *specials, int *tokens_amt);

#endif
