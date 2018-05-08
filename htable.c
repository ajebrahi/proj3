#include <stdio.h>
#include "ht_funcs.h"
#include<stdlib.h>
#include<string.h>
int main(int argvc, char *argv[]){

    FILE *file = NULL;
    int c;
    int freq[256] = {0};
    char *codes[256];
    int i;
    char *initial_code = NULL;
    Node *head = NULL;
    file = fopen(argv[1], "r");

    if(file == NULL){
        printf("usage\n");
        return 0;
    }

    while((c = getc(file)) != EOF){
        freq[c]++;
    }

    head = build_linked(freq);
    head = build_tree(head);

    for(i = 0; i < 256; i++){
        codes[i] = (char *)malloc(sizeof(char)*2);
        strcpy(codes[i], "-");
    }
    initial_code = (char *)malloc(sizeof(char)*2);
    initial_code[0] = '\0';
    encode(codes, head, initial_code);
    free(initial_code);
    for(i = 0; i < 256; i++){
        if(codes[i][0] != '-'){
            printf("0x%02x: %s\n", i, codes[i]);
        }
    }

    free_tree(head);
    for(i = 0; i < 256; i++){
        free(codes[i]);
    }

    fclose(file);
    return 0;
}
