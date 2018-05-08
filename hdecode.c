#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht_funcs.h"

#define CHAR_RANGE  257
#define FAKE_EOF    256
#define CHAR_BITS   8

void Error(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

int *ReadHeader(FILE *in)
{
    static int frequencies[CHAR_RANGE];
    int i, count, letter, freq;

    if(fscanf(in, "%d", &count) != 1) Error("invalid input file.");

    for(i = 0; i < count; i++)
    {
        if((fscanf(in, "%d %d", &letter, &freq) != 2)
           || letter < 0 || letter >= CHAR_RANGE) Error("invalid input file.");

        frequencies[letter] = freq;
    }
    fgetc(in); /* discard last newline */

    return frequencies;
}

int ReadBit(FILE *in){
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


/* decode and return a single character from the input using the given Huffman
 *  *  * tree */
int DecodeChar(FILE *in, Node *tree)
{
    while(tree->left || tree->right)
    {
        if(ReadBit(in)) tree = tree->right;
        else tree = tree->left;

        if(!tree) Error("invalid input file.");
    }
    return tree->letter;
}
/* decode the Huffman-encoded file in and save the results to out */
void Decode(FILE *in, FILE *out)
{
    int *frequencies, c;
    Node *head;

    frequencies = ReadHeader(in);
    head = build_linked(frequencies);
    head = build_tree(head);

    while((c = DecodeChar(in, head)) != FAKE_EOF)
        fputc(c, out);

    free_tree(head);
}

int main(int argc, char *argv[])
{
    FILE *in, *out;

    if(argc != 3){
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        exit(0);
    }
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    Decode(in, out);

    fclose(in);
    fclose(out);

    return 0;
}

