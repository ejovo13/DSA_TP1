#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node_t {
    int val;
    struct node_t *next;
} Node;

typedef struct list_t {

    Node *head;
    Node *last;

} List;

Node *newNode(int __val);

List *newList();

List *appendList(List *__list, int __val);

size_t lenList(const List *__list);

Node *atList(const List *__list, int __index);

Node *penultimateNode(const Node *__node);

Node *secondToLast(const List *__list);

// Extract an element from the list given a certain index,
// REMOVING the element from the linked list
Node *extractNode(List *__list, int __index);

// Free the list and then return a null pointer.
// thus we can call some thing like
//
// list = freeList(list);
//
// Alternatively, use the function releaseList(**List) to reset the
// pointer to NULL:
//
// releaseList(&list);
List *freeList(List *__list);

// release and reset to NULL
void releaseList(List **__plist);

// append element to end of the node
// and return a pointer to the node NEWLY_CREATED_ELEMENT. If __node
// is originally null, we HAVE to reassign it when calling
// appendNode. Therefore, it is best practice to call
// node = appendNode(node, 10).
Node *appendNode(Node *__node, int __val);

// Free all of the node's elements and set __node to NULL
void releaseNode(Node **__pnode);

// recursively free all of the nodes in the node
void freeNode(Node *__node);

size_t lenNode(const Node *__node);

Node *atNode(const Node *__node, int __index);

// Remove the node and then return the removed node pointer.
// return NULL if we couldn't find it
Node *removeNode(Node *__node, int __val);

void printList(const List *__list);

Node *removeNodePtr(Node *__node, Node *__rem);

Node *extractNodePtr(List *__list, Node *__rem);

Node *getNode(const List *__list, int __val);

bool isElementList(const List *__list, int __val);