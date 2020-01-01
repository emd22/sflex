#include "sflex.h"
#include <stdio.h>

int main() {
    char test_data[] = "   Hello SFLEX!   this+is a   test\n\t of   the*single   function    \n\tlexer! ";
    const char *delims = "+*";
    token_t tokens[256];
    int token_i, i;
    
    token_i = sflex(test_data, delims, tokens);
    for (i = 0; i < token_i; i++) {
        printf("Token: [%.*s]\n", sflex_len(tokens[i]), sflex_tok(tokens[i]));
    }
    
    return 0;
}
