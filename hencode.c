#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht_funcs.c"

#define CHAR_RANGE  257
#define FAKE_EOF    256
#define CHAR_BITS   8


/* output the Huffman header for an encoded file */
void WriteHeader(FILE *out, int frequencies[])
{
    int i, count = 0;
    
    for(i = 0; i < CHAR_RANGE; i++) if(frequencies[i]) count++;
    fprintf(out, "%d\n", count);
    
    for(i = 0; i < CHAR_RANGE; i++)
        if(frequencies[i]) fprintf(out, "%d %d\n", i, frequencies[i]);
}


/* write the given bit encoding to the output file */
void WriteBits(const char *encoding, FILE *out)
{
    /* buffer holding raw bits and number of bits filled */
    static int bits = 0, bitcount = 0;
    
    while(*encoding)
    {
        /* push bits on from the right */
        bits = bits * 2 + *encoding - '0';
        bitcount++;
        
        /* when we have filled the char, output as a single character */
        if(bitcount == CHAR_BITS)
        {
            fputc(bits, out);
            bits = 0;
            bitcount = 0;
        }
        
        encoding++;
    }
}

/* read a single bit from the input file */
int ReadBit(FILE *in)
{
    /* buffer holding raw bits and size of MSB filled */
    static int bits = 0, bitcount = 0;
    int nextbit;
    
    if(bitcount == 0)
    {
        bits = fgetc(in);
        bitcount = (1 << (CHAR_BITS - 1));
    }
    
    nextbit = bits / bitcount;
    bits %= bitcount;
    bitcount /= 2;
    
    return nextbit;
}

/* create a Huffman encoding for the file in and save the encoded version to
 *  * out */
void Encode(FILE *in, FILE *out){
    int c;
    int freq[256] = {0};
    char *codes[257];
    int i;
    char *initial_code = NULL;
    Node *head = NULL;

    while((c = getc(in)) != EOF){
        freq[c]++;
    }

    head = build_linked(freq);
    head = build_tree(head);

    for(i = 0; i < 257; i++){
        codes[i] = (char *)malloc(sizeof(char)*2);
        strcpy(codes[i], "-");
    }
    initial_code = (char *)malloc(sizeof(char)*2);
    initial_code[0] = '\0';
    encode(codes, head, initial_code);


    /* use FAKE_EOF to indicate end of input */
    WriteBits(codes[FAKE_EOF], out);
    
    /* write an extra 8 blank bits to flush the output buffer */
    WriteBits("0000000", out);
    
    free(initial_code);
    for(i = 0; i < 257; i++){
        free(codes[i]);
    }
    free_tree(head);
}

/* program to encode and decode files using Huffman trees */
int main(int argc, char *argv[])
{
    FILE *in, *out;
    
    if(argc != 3){
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        exit(0);
    }
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    Encode(in, out);

    fclose(in);
    fclose(out);
    
    return 0;
}
