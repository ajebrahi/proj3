#include "ht_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

