#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
    void* element;
    struct Node* next;
}Node;

Node* createList() {
    Node* node = (Node*) malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(-1);
    }
    node->element = NULL;
    node->next = NULL;
    return node;
}

int size(Node* list) {
    if (list->element == NULL) {
        return 0;
    }
    int sum = 1;
    while(list->next != NULL) {
        sum++;
        list = list->next;
    } 
    return sum;
}

void append(Node* list, void* el) {
    if (el == NULL) {
        fprintf(stderr, "Cannot append NULL element\n");
        return;
    }
    if (size(list) == 0) {
        list->element = el;
        return;
    }
    while (list->next != NULL) {
        list = list->next;
    }
    list->next = (Node*) malloc(sizeof(Node));
    if (list->next == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(-1);
    }
    list->next->element = el;
    list->next->next = NULL;
}

void* getElement(Node* list, int index) {
    int length = size(list);
    if (index >= length || index < 0) {
        fprintf(stderr, "Index out of bounds: %d\n", index);
        return NULL;
    }
    
    for (int i = 0; i < index; i++) {
        list = list->next;
    }

    return list->element;
}

void freeList(Node* list){
    if(list == NULL) {
	return;
    }	    
    if(list->next != NULL) {
	freeList(list->next);
    }
    free(list);
}
