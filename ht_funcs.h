typedef struct Node{

    int letter;
    int frequency;
    struct Node *next;
    struct Node *left;
    struct Node *right;

} Node;

Node *insert_sorted(Node *head, Node *new);
Node *build_linked(int arr[]);
Node *build_tree(Node *head);
void encode(char *arr[], Node *root, char *code);
void free_tree(Node *root);
