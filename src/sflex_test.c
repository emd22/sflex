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
    sflex_t inst;
    inst = sflex(data, "+-*/=:;(){}", SFLEX_USE_STRINGS);
    
    int i;
    for (i = 0; i < inst.token_amt; i++) {
        printf("Token: [%.*s]\n", sflex_len(inst.tokens[i]), sflex_tok(inst.tokens[i]));
    }
    
    sflex_destroy(&inst);
    
    return 0;
}
