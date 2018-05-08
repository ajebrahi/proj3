#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_RANGE  257
#define FAKE_EOF    256
#define CHAR_BITS   8

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

/* decode and return a single character from the input using the given Huffman
 *  *  * tree */
int DecodeChar(FILE *in, htree *tree)
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
    htree *tree;

    frequencies = ReadHeader(in);
    tree = BuildTree(frequencies);

    while((c = DecodeChar(in, tree)) != FAKE_EOF)
        fputc(c, out);

    FreeTree(tree);
}

