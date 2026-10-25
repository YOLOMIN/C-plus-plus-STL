#include <stdio.h>
#include <stdlib.h>

struct Node{
    int  val;
    struct Node *next;
};

// struct Node * push_front(struct Node **head,int v){
void push_front(struct Node **head,int v){
    
    struct Node* node = malloc(sizeof(struct Node));

    if(node == NULL) return *head;

    node->val = v;
    node->next = *head;

    head = node;

    // return node;
}

int main(){

    // struct Node* node1 = malloc(sizeof(struct Node));
    // struct Node* node2 = malloc(sizeof(struct Node));
    // struct Node* node3 = malloc(sizeof(struct Node));

    // node1->val = 10;
    // node1->next =  node2;

    // node2->val = 20;
    // node2->next = node3;

    // node3->val = 30;
    // node3->next = NULL;

    struct Node* head = NULL;
    head = push_front(&head,10);
    head = push_front(&head,20);
    head = push_front(&head,30);

    struct Node* p = head;
    while (p != NULL)
    {
        printf("%d\n",p->val);
        p = p->next;
    }

    p = head;
    while(p != NULL){
        struct Node *  next = p->next;
        free(p);
        p = next;

    }
    // free(node1);
    // free(node2);
    // free(node3);
}