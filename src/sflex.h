#ifndef SFLEX_H
#define SFLEX_H

#define sflex_len(token) (token.end-token.start)
#define sflex_tok(token) (token.start)

typedef struct {
    char *start;
    char *end;
} token_t;

int sflex(char *data, const char *specials, token_t *tokens);

#endif
