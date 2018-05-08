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

void encode(char *arr[], Node *root, char *code){

    char *left_path;
    char *right_path;
    if(root == NULL){ 
        return;
    }
    left_path = (char *)malloc(sizeof(char)*strlen(code)+2);
    right_path = (char *)malloc(sizeof(char)*strlen(code)+2);
    strcpy(left_path, code);
    strcat(left_path, "0");
    strcpy(right_path, code);
    strcat(right_path, "1");

    if(root->left == NULL && root->right == NULL){
        arr[root->letter] = realloc(arr[root->letter],sizeof(char) * strlen(code)*2);
        strcpy(arr[root->letter], code);
    }
    if(root->left != NULL){
        encode(arr, root->left, left_path);}
    if(root->right != NULL){
        encode(arr, root->right, right_path);}

    free(left_path);
    free(right_path);
}

void WriteHeader(FILE *out, int frequencies[])
{
    int i, count = 0;
    
    for(i = 0; i < CHAR_RANGE; i++){
        if(frequencies[i]) count++;
        fprintf(out, "%d\n", count);
    }
    for(i = 0; i < CHAR_RANGE; i++)
        if(frequencies[i]) fprintf(out, "%d %d\n", i, frequencies[i]);
}


void WriteBits(const char *encoding, FILE *out)
{
    static int bits = 0, bitcount = 0;
    
    while(*encoding)
    {
        bits = bits * 2 + *encoding - '0';
        bitcount++;
        
        if(bitcount == CHAR_BITS)
        {
            fputc(bits, out);
            bits = 0;
            bitcount = 0;
        }
        
        encoding++;
    }
}

int ReadBit(FILE *in)
{
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
    int freq[257] = {0};
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

    strcpy(codes[FAKE_EOF], "1");
    rewind(in);

    initial_code = (char *)malloc(sizeof(char)*2);
    initial_code[0] = '\0';
    encode(codes, head, initial_code);
    WriteHeader(out, freq);
    
    while((c = fgetc(in)) != EOF)
        WriteBits(codes[c], out);

    WriteBits(codes[FAKE_EOF], out);
    WriteBits("0000000", out);
    
    free(initial_code);
    for(i = 0; i < 257; i++){
        free(codes[i]);
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

    Encode(in, out);

    fclose(in);
    fclose(out);
    
    return 0;
}
