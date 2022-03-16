#include "tree.h"

#include <assert.h>

void t_addKey();
void t_removeKey();
void t_releaseBST(); // Create a new fisher-yates binary search tree and then release its elements

int main() {

    t_addKey();
    t_releaseBST();
    t_removeKey();

    return 0;
}

void t_addKey() {

    printf("Testing...\n");
    assert(true);

    BinTree *root = newBinTree(10);

    addKeyBST(root, 1); // Adds 1 to the left of 10
    addKeyBST(root, 0); // Adds 0 to the left of 10
    addKeyBST(root, 13); // Adds 13 to the left of 10
    addKeyBST(root, 5); // Adds 5 to the right of 1
    addKeyBST(root, 8);
    addKeyBST(root, 25);
    addKeyBST(root, 3);

    // assert that the nodes are in the correct order!
    assert(countNodes(root) == 8);

    assert(root->key == 10);
    assert(root->left->key == 1);
    assert(root->left->left->key == 0);
    assert(root->right->key == 13);
    assert(root->right->right->key == 25);
    assert(root->left->right->key == 5);
    assert(root->left->right->right->key == 8);
    assert(root->left->right->left->key == 3);

    releaseBST(&root);

    assert(!root);
}

void t_removeKey() {

    BinTree *root = newBinTree(10);

    addKeyBST(root, 1); // Adds 1 to the left of 10
    addKeyBST(root, 0); // Adds 0 to the left of 10
    addKeyBST(root, 13); // Adds 13 to the left of 10
    addKeyBST(root, 5); // Adds 5 to the right of 1
    addKeyBST(root, 8);
    addKeyBST(root, 25);
    addKeyBST(root, 3);

    // Here we are going to use removeKey and then free the resulting node
    BinTree *rem = NULL;

    /**========================================================================
     *!                           Remove key 3
     *========================================================================**/

    rem = removeKey(root, 3);

    assert(countNodes(root) == 7);
    assert(root->key == 10);
    assert(root->left->key == 1);
    assert(root->left->left->key == 0);
    assert(root->right->key == 13);
    assert(root->right->right->key == 25);
    assert(root->left->right->key == 5);
    assert(root->left->right->right->key == 8);

    free(rem);

    /**========================================================================
     *!                           Remove key 13
     *========================================================================**/

    rem = removeKey(root, 13);

    assert(countNodes(root) == 6);
    assert(root->key == 10);
    assert(root->left->key == 1);
    assert(root->left->left->key == 0);
    assert(root->right->key == 25);
    assert(root->left->right->key == 5);
    assert(root->left->right->right->key == 8);

    free(rem);

    /**========================================================================
     *!                           Remove key 1
     *========================================================================**/

    rem = removeKey(root, 1);

    assert(countNodes(root) == 5);
    assert(root->key == 10);
    assert(root->left->left->key == 0);
    assert(root->right->key == 25);
    assert(root->left->key == 5);
    assert(root->left->right->key == 8);

    free(rem);

    /**========================================================================
     *!                           Remove key 10j
     *========================================================================**/

    rem = removeKey(root, 10);

    assert(countNodes(root) == 4);
    assert(root->key == 25);
    assert(root->left->left->key == 0);
    assert(root->left->key == 5);
    assert(root->left->right->key == 8);

    free(rem);

    releaseBST(&root);

    printf("Verified removal\n");
}

void t_releaseBST() {

    int n = 500;

    // create a new random tree, and then release it.
    BinTree *root = createRandomTree(n);

    assert(root);
    assert(countNodes(root) == n);

    // release the memory associated, and make sure that there are no memory leaks with valgrind
    releaseBST(&root);

}