#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct binary_tree_t {

    int key;
    struct binary_tree_t *right;
    struct binary_tree_t *left;

} BinTree;

// Create a new root for a binary tree whose data value is
// __root
BinTree *newBinTree(int __root) {

    BinTree *bst = (BinTree *) malloc(sizeof(BinTree));
    if (!bst) return NULL; // If allocation failed, return NULL

    bst->key = __root;
    bst->left = NULL;
    bst->right = NULL;

    return bst;
}

// This function assumes that the pointer __bst is not NULL and points to
// a valid root that has been allocated
void addKeyBST(BinTree *__bst, int __key) {

    if (__key <= __bst->key) {
        // If there is no node to the left,
        if (!__bst->left) {
            __bst->left = newBinTree(__key);
        } else {
            addKeyBST(__bst->left, __key);
        }
    } else {

        if (!__bst->right) {
            __bst->right = newBinTree(__key);
        } else {
            addKeyBST(__bst->right, __key);
        }
    }
}

// Let's remove a key and free the node.
void removeKeyBST(BinTree *__bst, int __key) {

    // Handle the case where  this


}


void printBST(const BinTree *__bst) {



}

void writeNode(const BinTree *__bst, FILE *__file) {

    if (!__bst) return;
    fprintf(__file, "  %d", __bst->key);
    if (!__bst->left && !__bst->right) {
        fprintf(__file, "\n");
        return;
    }
    if (__bst->left && __bst->right) fprintf(__file, " -> { %d %d }\n", __bst->left->key, __bst->right->key);
    else if (__bst->left) fprintf(__file, " -> %d\n", __bst->left->key);
    else if (__bst->right) fprintf(__file, " -> %d\n", __bst->right->key);

    if (__bst->left) writeNode(__bst->left, __file);
    if (__bst->right) writeNode(__bst->right, __file);
}

// I actually want to create a dot output

void createDotBST(const BinTree *__bst, const char *__filename) {

    FILE *f = fopen(__filename, "w");

    fprintf(f, "digraph {\n");

    writeNode(__bst, f);

    fprintf(f, "}\n");
    fclose(f);

}

BinTree *getParent(BinTree *__bst, int __key) {

    if (!__bst) return NULL;

    if (__bst->key == __key) return __bst;

    if (__bst->right && __bst->right->key == __key) return __bst;
    if (__bst->left  &&  __bst->left->key == __key) return __bst;

    if (__bst->right) return getParent(__bst->right, __key);
    if (__bst->left)  return getParent(__bst->left,  __key);

}

// Before I can do that, I want a helper routine that will get the next largest number.
// Get the next largest key.
BinTree *getNextKey(const BinTree *__bst) {

    BinTree *it = __bst->right;

    while (it->left) {
        it = it->left;
    }

    return it;

}

// Return a pointer to the node that contains the key entered as an argument
BinTree *getKey(BinTree *__bst, int __key) {

    // printf("Visiting key: %d\n", __bst->key);

    if (__bst->key == __key) return __bst;

    if (__bst->right) getKey(__bst->right, __key);
    if (__bst->left)  getKey(__bst->left, __key);

    // printf("Reached end of the line, now returning NULL\n");
    // return NULL;
}

bool nodeIsLeaf(const BinTree *__bst) {
    return (!__bst->left && !__bst->right);
}

bool nodeOnLeft(const BinTree *__parent, const BinTree *__child) {
    return __child->key <= __parent->key;
}

int numChildren(const BinTree *__bst) {

    int count = 0;
    if (__bst->right) count ++;
    if (__bst->left) count ++;

    return count;

}

// bool hasChild(const BinTree *__bst)

// Need to implement a procedure to REMOVE a node from a tree.
BinTree *removeKey(BinTree *__bst, int __key) {

    // First and foremost I need to locate the actual key.
    BinTree *node = getKey(__bst, __key);
    if (!node) return NULL;
    BinTree *parent = getParent(__bst, __key);

    // 1. The node is a leaf
    if (nodeIsLeaf(node)) {
        if (nodeOnLeft(node, parent)) { // then the node is on the left of the parent
            parent->left = NULL; // remove from the tree.
        } else {
            parent->right = NULL;
        }
    }

    // 2. The node has a single child
    if (numChildren(node) == 1) {
        if (node->left) {
            if (nodeOnLeft(node, parent)) {
                parent->left = node->left;
            } else {
                parent->right = node->left;
            }
        } else if (node->right) {
            if (nodeOnLeft(node, parent)) {
                parent->left = node->right;
            } else {
                parent->right = node->right;
            }
        }
    }

    // 2. The node has two children. In this case, we find the next
    // largest key and replace that with the node we want to delete, and then remove
    // the next largest key.


    // Need to consider some cases...
    // if the node to remove is the root...
    // if (node == __bst) {
    //     BinTree *next = getNextKey(__bst);
    //     BinTree *parent = getParent(__bst, next->key);



    // } else if (true) {

    // }


}

// BinTree *getParent(BinTree *__bst, int __key) {

//     if (!__bst) return NULL;

//     if (__bst->key == __key) return __bst;

//     if (__bst->right && __bst->right->key == __key) return __bst;
//     if (__bst->left  &&  __bst->left->key == __key) return __bst;

//     if (__bst->right) return getParent(__bst->right, __key);
//     if (__bst->left)  return getParent(__bst->left,  __key);

// }

