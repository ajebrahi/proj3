#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include "ht_funcs.h"*/

#define CHAR_RANGE  257
#define FAKE_EOF    256
#define CHAR_BITS   8
typedef struct Node{

    int letter;
    int frequency;
    struct Node *next;
    struct Node *left;
    struct Node *right;

} Node;
Node *insert_sorted(Node *head, Node *cur){

    Node *prev, *next;
    if(head == NULL){
        free(head);
        cur->next = NULL;
        return cur;
    }
    prev = head;
    next = head->next;
    while((next != NULL && cur->frequency > next->frequency) || 
         (next != NULL && (cur->frequency == next->frequency) && (cur->letter > next->letter))){
        prev = prev->next;
        next = next->next;
    }
    prev->next = cur;
    cur->next = next;
    return head;
}


Node *build_linked(int arr[]){
    Node *new;
    int i;
    Node *head = NULL;;

    for (i = 0; i < 256; i++) {
        if (arr[i] > 0) {
            new = (Node *)malloc(sizeof(Node));
            new->letter = i;
            new->frequency = arr[i];
            new->left = NULL;
            new->right = NULL;
            head = insert_sorted(head, new);
        }
    }

    return head;
}

Node *build_tree(Node *head){
    Node *first = NULL, *second = NULL, *new = NULL;
    Node *tmp;
    if (head == NULL) {
        return head;
    }
    first = head;
    second = head->next;
    while (head->next != NULL) {
        first = (Node *)malloc(sizeof(Node));
        second = (Node *)malloc(sizeof(Node));
        if (head != NULL) {
            first->letter = head->letter;
            first->frequency = head->frequency;
            first->left = head->left;
            first->right = head->right;
            first->next = NULL;
        }
        if (head->next != NULL) {
            second->letter = head->next->letter;
            second->frequency = head->next->frequency;
            second->left = head->next->left;
            second->right = head->next->right;
            second->next = NULL;
        }
        new = (Node *)malloc(sizeof(Node));
        new->frequency = first->frequency + second->frequency;
        new->letter = ((first->letter < second->letter) ? first->letter : second->letter);
        new->left = first;
        new->right = second;
        tmp = head;
        head = head->next;
        free(tmp);
        tmp = head;
        head = head->next;
        free(tmp);
        head = insert_sorted(head, new);
    }
    return head;
}

void free_tree(Node *root){

    if(root == NULL){
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    
    free(root);

}

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

int DecodeChar(FILE *in, Node *tree)
{
    if(!tree) Error("ahhh");
    while(tree->left || tree->right)
    {
        if(ReadBit(in)) tree = tree->right;
        else tree = tree->left;

        if(!tree) Error("invalid input file.");
    }
    return tree->letter;

}

void Decode(FILE *in, FILE *out)
{
    int *frequencies, c;
    Node *head;

    frequencies = ReadHeader(in);
    head = build_linked(frequencies);
    head = build_tree(head);

    while((c = DecodeChar(in, head)) != FAKE_EOF){
        fputc(c, out);
    }

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

