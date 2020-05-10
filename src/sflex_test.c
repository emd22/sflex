#include "sflex.h"
#include <stdlib.h>
#include <stdio.h>

char *load_file(void) {
    FILE *fp = fopen("token_test.txt", "rb");
    if (fp == NULL)
        return NULL;
        
    fseek(fp, 0, SEEK_END);
    unsigned file_size = ftell(fp);
    rewind(fp);
    
    char *buffer = malloc(file_size);
    fread(buffer, 1, file_size, fp);
    fclose(fp);
    return buffer;
}

int main() {
    char *data;
    if ((data = load_file()) == NULL) {
        printf("Could not load file\n");
        return 1;
    }
    token_t tokens[256];
    int token_i, i;
    
    token_i = sflex(data, "+-*/=:;()", tokens);
    for (i = 0; i < token_i; i++) {
        printf("Token: [%.*s]\n", sflex_len(tokens[i]), sflex_tok(tokens[i]));
    }
    
    return 0;
}
