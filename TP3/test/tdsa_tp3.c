#include "tree.h"

#include <assert.h>

BinTree *TEST_TREE();
void t_addKey();
void t_removeKey();
void t_releaseBST(); // Create a new fisher-yates binary search tree and then release its elements
void t_isElement();

int main() {

    t_addKey();
    t_releaseBST();
    t_removeKey();
    t_isElement();

    return 0;
}

BinTree *TEST_TREE() {

    BinTree *root = newBinTree(10);

    addKeyBST(root, 1);  // Adds 1 to the left of 10
    addKeyBST(root, 0);  // Adds 0 to the left of 10
    addKeyBST(root, 13); // Adds 13 to the left of 10
    addKeyBST(root, 5);  // Adds 5 to the right of 1
    addKeyBST(root, 8);
    addKeyBST(root, 25);
    addKeyBST(root, 3);

    return root;

}


void t_addKey() {

    printf("Testing...\n");
    assert(true);

    BinTree *root = TEST_TREE();

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

    BinTree *root = TEST_TREE();

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

    assert(rem);
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

    assert(rem);
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

    assert(rem);
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

    assert(rem);
    free(rem);


    rem = removeKey(root, 100);

    assert(!rem); // verify that no removal was performed

    releaseBST(&root);

    rem = removeKey(root, 100); // verify that calling removeKey on a null tree does nothing
    assert(!rem);

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

void t_isElement() {

    BinTree *root = TEST_TREE();

    // valid elements
    assert(isElement(root, 10));
    assert(isElement(root, 0));
    assert(isElement(root, 1));
    assert(isElement(root, 13));
    assert(isElement(root, 5));
    assert(isElement(root, 8));
    assert(isElement(root, 25));
    assert(isElement(root, 3));

    // nonsense elements
    assert(!isElement(root, 2));
    assert(!isElement(root, 12));
    assert(!isElement(root, 4));
    assert(!isElement(root, 6));
    assert(!isElement(root, 7));
    assert(!isElement(root, 9));
    assert(!isElement(root, -1));

    releaseBST(&root);
}