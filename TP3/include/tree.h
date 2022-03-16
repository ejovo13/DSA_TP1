#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ejovo_rand.h"


typedef struct binary_tree_t {

    int key;
    struct binary_tree_t *right;
    struct binary_tree_t *left;

} BinTree;

// A Tourist visits the nodes of a BinTree and keeps track of how long they've been travelling
// Tourists are used when deleting a key and we need to know how many levels I went down to find a
// nodes replacement
typedef struct tourist_t {

    const struct binary_tree_t *this;
    int count;

} Tourist;

Tourist *newTourist(const BinTree *__bst);

Tourist visitRight(Tourist __t);

Tourist visitLeft(Tourist __t);

// Create a new root for a binary tree whose data value is
// __root
BinTree *newBinTree(int __root);

// This function assumes that the pointer __bst is not NULL and points to
// a valid root that has been allocated
void addKeyBST(BinTree *__bst, int __key);

// Let's remove a key and free the node.
void removeKeyBST(BinTree *__bst, int __key);

void releaseBST(BinTree **__bst);

// recursively release the memory associated with this node,
// performing 0 checks
void releaseBST_(BinTree *__bst);


void printBST(const BinTree *__bst);

void writeNode(const BinTree *__bst, FILE *__file);

// I actually want to create a dot output

void createDotBST(const BinTree *__bst, const char *__filename);

void getParent_(BinTree *__bst, int __key, BinTree **__out);

BinTree *getParent(BinTree *__bst, int __key);

// Before I can do that, I want a helper routine that will get the next largest number.
// Get the next largest key.
BinTree *getNextKey(const BinTree *__bst);

void getKey_(BinTree *__bst, int __key, BinTree **__out);
// Return a pointer to the node that contains the key entered as an argument
BinTree *getKey(BinTree *__bst, int __key);


bool nodeIsLeaf(const BinTree *__bst);

bool nodeOnLeft(const BinTree *__parent, const BinTree *__child);

int numChildren(const BinTree *__bst);

BinTree *getOnlyChild(const BinTree *__parent);

// Take the chain 10 -> 13 -> 25 as an example. collapseChain(parent) => 10 -> 25,
// where parent has the key 10.
// parent may have two children and __key specifies which direction to collapse.
// The child with key value __key must only have one child
// return the middle value that is popped.
BinTree *collapseChain(BinTree *__parent, int __key);

// bool hasChild(const BinTree *__bst)

// Need to implement a procedure to REMOVE a node from a tree.
// This prodcedure will effectively pop the target key from the BST.
// If you would instead like to free the key, call deleteKey()
BinTree *removeKey(BinTree *__bst, int __key);

// I would like a special case to remove a key that is the head.
// Return a pointer to the old head and change the root pointer
// in place
// BinTree *removeHead(BinTree **__bst);

void deleteKey(BinTree *__bst, int __key);

// Create a random binary search tree with the elements of a randomly suffled set of the integers;
BinTree *createRandomTree(int __n);

int heightBST(const BinTree *__root);

int deqFactor(const BinTree *__root);

// Make use of a "Tourist" to visit the nodes and keep track of how many nodes
// visited
void depthKey_(const BinTree *__root, int __key, Tourist __t, int *__count);

// Return the depth of the key that contains the node
int depthKey(const BinTree *__root, int __key);

// count the number of nodes in a binary search tree.
int countNodes(const BinTree *__root);