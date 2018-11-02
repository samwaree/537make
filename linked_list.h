#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node Node;

Node* createList();
int size(Node*);
void append(Node*, void*);
void* getElement(Node*, int);
#endif
