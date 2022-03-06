#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ejovo_rand.h"

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

void getParent_(BinTree *__bst, int __key, BinTree **__out) {

    if (!__bst) return;

    if (__bst->key == __key) {
        *__out = __bst;
        return;
    };

    if (__bst->right && __bst->right->key == __key) {
        *__out = __bst;
        return;
    }
    if (__bst->left  &&  __bst->left->key == __key) {
        *__out = __bst;
        return;
    }

    if (__bst->right) getParent_(__bst->right, __key, __out);
    if (__bst->left)  getParent_(__bst->left,  __key, __out);



}

BinTree *getParent(BinTree *__bst, int __key) {

   BinTree *out = NULL;

   getParent_(__bst, __key, &out);

   return out;

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

void getKey_(BinTree *__bst, int __key, BinTree **__out) {

    if (__bst->key == __key) {
        *__out = __bst;
        return;
    }

    if (__bst->right) getKey_(__bst->right, __key, __out);
    if (__bst->left ) getKey_(__bst->left , __key, __out);

}
// Return a pointer to the node that contains the key entered as an argument
BinTree *getKey(BinTree *__bst, int __key) {

    BinTree *out = NULL;

    getKey_(__bst, __key, &out);

    return out;
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

BinTree *getOnlyChild(const BinTree *__parent) {

    if (!__parent) return NULL;

    if (__parent->left) return __parent->left;
    if (__parent->right) return __parent->right;

    return NULL;

}

// Take the chain 10 -> 13 -> 25 as an example. collapseChain(parent) => 10 -> 25,
// where parent has the key 10.
// parent may have two children and __key specifies which direction to collapse.
// The child with key value __key must only have one child
// return the middle value that is popped.
BinTree *collapseChain(BinTree *__parent, int __key) {

    BinTree *child = NULL;

    bool isOnLeftSide = false; // check if the child that we want is on the left side of the parent.

    if (__parent->right && __parent->right->key == __key) child = __parent->right;
    if (__parent->left  && __parent->left ->key == __key) {
        child = __parent->left ;
        isOnLeftSide = true;
    }

    if (isOnLeftSide) {
        __parent->left = getOnlyChild(child);
    } else {
        __parent->right = getOnlyChild(child);
    }

    child->right = NULL;
    child->left = NULL;

    return child;
}

// bool hasChild(const BinTree *__bst)

// Need to implement a procedure to REMOVE a node from a tree.
// This prodcedure will effectively pop the target key from the BST.
// If you would instead like to free the key, call deleteKey()
BinTree *removeKey(BinTree *__bst, int __key) {

    // First and foremost I need to locate the actual key.
    BinTree *node = getKey(__bst, __key);

    printf("Node found with key: %d\n", node->key);

    if (!node) {
        printf("ERROR - couldnt find key %d\n", __key);
        return NULL;
    }
    BinTree *parent = getParent(__bst, __key);

    printf("Removing key %d with parent %d\n", node->key, parent->key);

    // 1. The node is a leaf
    if (nodeIsLeaf(node)) {

        printf("Node is a LEAF\n");

        if (parent->left->key == node->key) { // then the node is on the left of the parent
            printf("Node is on the left\n");
            parent->left = NULL; // remove from the tree.
            return node;
        } else {
            parent->right = NULL;
            return node;
        }
    }

    // 2. The node has a single child
    if (numChildren(node) == 1) {

        return collapseChain(parent, node->key);

        // printf("Node has ONE child\n");
        // if (node->left) {
        //     if (nodeOnLeft(parent, node)) {
        //         parent->left = node->left;
        //         return node;
        //     } else {
        //         parent->right = node->left;
        //         return node;
        //     }
        // } else if (node->right) {
        //     // printf("Replacing %d with node on the right, %d\n", node->key, next->key)
        //     // ");
        //     // !! I SHOULD BE REPLACING THE RIGHT SIDE HERE...
        //     if (nodeOnLeft(parent, node)) {
        //         printf("child: %d <= parent: %d\n", node->key, parent->key);
        //         printf("Replacing left side of %d  with right side of %d\n", parent->key, node->key);
        //         parent->left = node->right;
        //         return node;
        //     } else {
        //         printf("Replacing right side of %d  with right side of %d\n", parent->key, node->key);
        //         parent->right = node->right;
        //         return node;
        //     }
        // }
    }

    // 3. The node has two children. In this case, we find the next
    // largest key and replace that with the node we want to delete, and then remove
    // the next largest key.

    if (numChildren(node) == 2) {
        printf("Node has TWO children\n");

        BinTree *next = getNextKey(node);

        printf("Replacing %d with %d\n", node->key, next->key);

        int tmp = node->key;
        node->key = next->key;
        next->key = tmp;
        // switch the values

        printf("Top node now contains %d, bottom node: %d\n", node->key, next->key);

        // let's handle the removal manually instead of relying on the removeKey function
        // at this stage we've reversed the two values.




        // And now remove the same value
        return removeKey(getParent(node, tmp), tmp);

    }

}

// I would like a special case to remove a key that is the head.
// Return a pointer to the old head and change the root pointer
// in place
// BinTree *removeHead(BinTree **__bst) {

//     BinTree *root = *__bst;

//     removeKey(root, root->key);

//     printf("New head key: %d\n", root->key);

//     return NULL;

// }

void deleteKey(BinTree *__bst, int __key) {

    BinTree *node = removeKey(__bst, __key);
    free(node);

}

// Create a random binary search tree with the elements of a randomly suffled set of the integers {1, ... , n}
BinTree *createRandomTree(int __n) {

    int *fischer = fischer_yates(__n);

    BinTree *root = newBinTree(fischer[0]);

    for (int i = 1; i < __n; i++) {
        addKeyBST(root, fischer[i]);
    }

    free(fischer);

    return root;
}

int heightBST(const BinTree *__root) {

    if (!__root) return 0;

    int left_height = heightBST(__root->left);
    int right_height = heightBST(__root->right);

    return (left_height > right_height ? left_height : right_height) + 1; // return max(left, right) + 1

}

int deqFactor(const BinTree *__root) {

    if (!__root) return 0;

    return abs(heightBST(__root->left) -  heightBST(__root->right));
}