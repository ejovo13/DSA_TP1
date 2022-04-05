// Implementation of an AVL self-balancing binary search tree

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// First thing that I need is a new node structure I guess.


// I have a big list of API that I'm going to have to implement
// - [ ] Print the tree
// - [ ] create a graphviz of the tree
// - [ ] rotateRight
// - [ ] rotateLeft
// - [ ] insertAVL

#define DATA_TYPE double

typedef struct node_t {

    int key;

    struct node_t *left;
    struct node_t *right;

    int height;
    int deq_factor;

    DATA_TYPE *data;

} Node;

// Allocate the space for a new node
Node *newNode(int __key) {

    Node *n = (Node *) malloc(sizeof(Node));

    if (!n) return NULL;

    n->left = NULL;
    n->right = NULL;
    n->data = NULL;
    n->key = __key;

    n->height = 0;
    n->deq_factor = 0;

    return n;
}

// This function assumes that the pointer __bst is not NULL and points to
// a valid root that has been allocated
void addKeyBST(Node *__bst, int __key) {

    if (__key <= __bst->key) {
        // If there is no node to the left,
        if (!__bst->left) {
            __bst->left = newNode(__key);
        } else {
            addKeyBST(__bst->left, __key);
        }
    } else {

        if (!__bst->right) {
            __bst->right = newNode(__key);
        } else {
            addKeyBST(__bst->right, __key);
        }
    }
}
// return true is we have no descendants
bool isLeaf(const Node *__node) {
    bool a = __node->left == NULL;
    bool b = __node->right == NULL;
    return a && b;
}

// simple xor operation
bool hasOneChild(const Node *__node) {
    bool a = __node->left == NULL;
    bool b = __node->right == NULL;
    return a != b;
}

// if we are sure there is only one child, access it
Node *getSingleChild(const Node *__node) {
    if (__node->left) return __node->left;
    else return __node->right;
}

bool hasTwoChildren(const Node *__node) {
    bool a = __node->left != NULL;
    bool b = __node->right != NULL;
    return(a && b);
}

// this function should actually update the tree in place
int computeHeights(Node *__node) {

    if (!__node) return -1;
    if (isLeaf(__node)) {
        __node->height = 0;
        return 0;
    }
    if (hasOneChild(__node)) {
        __node->height = 1 + computeHeights(getSingleChild(__node));
        return __node->height;
    }

    if (hasTwoChildren(__node)) {

        int h_left = computeHeights(__node->left);
        int h_right = computeHeights(__node->right);
        __node->height =  1 + (h_left > h_right ? h_left : h_right);
        return __node->height;
    }
    // return the max height of the left and right subtrees
}


int deqFactor(const Node *__root) {

    if (!__root) return 0;

    if (isLeaf(__root)) return 0;

    if (hasOneChild(__root)) {
        if (__root->left) return __root->left->height + 1;
        else return -(__root->right->height + 1);
    }

    if (hasTwoChildren(__root)) {
        return (__root->left->height - __root->right->height);
    }
}

// I need to implement rotations. In order to do so, I need to find the parent of a certain node.
// If we want to get the parent of the root node, return the root node. But write something to the console?
// getParent via a DFS (because shit is way easier to implement)
void getParent_(Node *__bst, int __key, Node **__out) {

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

Node *getParent(Node *__bst, int __key) {

   Node *out = NULL;

   getParent_(__bst, __key, &out);

   return out;

}

void writeNode(const Node *__bst, FILE *__file) {

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

void createDotBST(const Node *__bst, const char *__filename) {

    FILE *f = fopen(__filename, "w");

    fprintf(f, "digraph {\n");

    writeNode(__bst, f);

    fprintf(f, "}\n");
    fclose(f);

}


// This function ASSUMES that __parent_node is non null and that one of its children matches the __key
Node *getChild(const Node *__parent_node, int __key) {

    if (__parent_node->left->key == __key) return __parent_node->left;
    if (__parent_node->right->key == __key) return __parent_node->right;

    return NULL;
}

bool isChildLeftKey(const Node *__parent_node, int __key) {
    return (__parent_node->key > __key);
}

bool isChildLeft(const Node *__parent_node, const Node *__child) {
    return (__parent_node->left == __child);
}

// When given an AVL tree and a key, perform a right rotation on the specified node
// returns a pointer to the new root
Node *rotRight(Node *__avl, int __key) {

    // Get parent and child
    Node *parent = getParent(__avl, __key);
    Node *child  = parent->key == __key ? parent : getChild(parent, __key);

    // printf("Parent key: %d\n", parent->key);
    // printf("Child key: %d\n", child->key);

    // if child has left null, exit immediately, return __avl
    if (!child->left) {
        fprintf(stderr, "Child has no left subtree, exiting rotation\n");
        return __avl;
    }

    // otherwise, carry on with our rotation
    Node *left = child->left;

    child->left = left->right;
    left->right = child;

    // if the __avl is the root node, don't update the parent-child connection (that is non existent)
    if (parent == child) return left;

    // otherwise replace parent-child connection
    if (isChildLeft(parent, child)) parent->left = left;
    else parent->right = left;

    // and return the original parent
    return __avl;
}

// When given an AVL tree and a key, perform a right rotation on the specified node
// returns a pointer to the new root
Node *rotLeft(Node *__avl, int __key) {

    // Get parent and child
    Node *parent = getParent(__avl, __key);
    printf("Parent add: %x\n", parent);
    printf("Parent key: %d\n", parent->key);
    printf("Parent left: %x\n", parent->left);
    printf("Parent right: %x\n", parent->left);

    Node *child  = parent->key == __key ? parent : getChild(parent, __key);

    printf("Child key: %d\n", child->key);


    // if child has left null, exit immediately, return __avl
    if (!child->right) {
        fprintf(stderr, "Child has no right subtree, exiting rotation\n");
        return __avl;
    }

    // otherwise, carry on with our rotation
    Node *right = child->right;

    child->right = right->left;
    right->left = child;

    // if the __avl is the root node, don't update the parent-child connection (that is non existent)
    if (parent == child) return right;

    // otherwise replace parent-child connection
    if (isChildLeft(parent, child)) parent->left = right;
    else parent->right = right;

    // and return the original parent
    return __avl;
}

// dfs search
void updateDeq(Node *__avl) {

    if (!__avl) return;

    __avl->deq_factor = deqFactor(__avl);

    updateDeq(__avl->left);
    updateDeq(__avl->right);

}

// an AVL is balanced if it passes the invariant deq(avl) \in {-1, 0, 1}
bool isBalanced(const Node *__avl) {

    // iterate along until we get false

    if (__avl == NULL) return true;

    int deq = __avl->deq_factor;
    bool inv = (deq == -1) || (deq == 0) || (deq == 1);

    return inv && isBalanced(__avl->left) && isBalanced(__avl->right);
}

// recompute the heights and update equilibrium factor
void updateBST(Node *__bst) {
    if (!__bst) return;
    computeHeights(__bst);
    updateDeq(__bst);
}

Node *reequilibrate_(Node *const __root, Node *__node) {
// Here I should really only need to pass in the parent!
    if (!__node) return NULL;

    if (isBalanced(__node)) return __root;


    if (__node->deq_factor == 2) {

        if (__node->left->deq_factor == 1) {

            // then simply perform a right rotation
            Node *temp = rotRight(__root, __node->key);
            updateBST(__root);
            return temp;

        }

        if (__node->left->deq_factor == -1) {

            printf("Need to perform a left rotation on: %d\n", __node->left->key);
            printf("and a right rotation on: %d\n", __node->key);
            // then rotate the left child to the left
            Node *temp = rotLeft(__node, __node->left->key);
            // printf("After rotation, temp->key : %d\n", temp->key);
            temp = rotRight(__root, __node->key);
            updateBST(__root);
            return temp;

        }
    } else if (__node->deq_factor == -2) {

        if (__node->right->deq_factor == -1) {
            printf("Perform left rotation on %d\n", __node->key);
            // then simply perform a right rotation
            Node *temp = rotLeft(__root, __node->key);
            updateBST(__root);
            return  temp;

        }

        if (__node->right->deq_factor == +1) {

            printf("Need to perform a right rotation on: %d\n", __node->left->key);
            printf("and a left rotation on: %d\n", __node->key);
            // then rotate the left child to the left
            rotRight(__node, __node->left->key);
            Node *temp = rotLeft(__root, __node->key);
            updateBST(__root);

            // printf("After rotation, temp->key : %d\n", temp->key);
            return rotLeft(__root, __node->key);
        }
    }

    // now go ahead and reequilibriate the children
    reequilibrate_(__node, __node->left);
    reequilibrate_(__node, __node->right);

    return __root;
}

// return a pointer to the new root
Node *reequilibrate(Node *__avl) {

    if (!__avl) return NULL;

    // If the tree is already balanced, do nothing
    if (isBalanced(__avl)) return __avl;

    return reequilibrate_(__avl, __avl);

}


// Update all the heights in this tree via DFS and compute the deq in the meantime
// return a pointer to the new node since we might be able to update the root
Node *updateAVL(Node *__avl) {

    // I really don't have time to optimize all of my functions, I really just
    // need a working version and that HAS to be enough for me....
    // calculate the height of each node
    updateBST(__avl);
    Node *out = reequilibrate(__avl);
    updateBST(__avl); // if not balanced, the call to requili

    return out;

}

Node *addKeyAVL(Node *__avl, int __key) {

    addKeyBST(__avl, __key);
    return updateAVL(__avl);
}


// void printHeights_(const Node *__avl);

// Visit all of the nodes via DFS and print their heights
void printHeights(const Node *__avl) {

    if (!__avl) return;

    printf("Visiting key: %d, H(key) := %d, deq: %d\n", __avl->key, __avl->height, __avl->deq_factor);

    printHeights(__avl->left);
    printHeights(__avl->right);

}

void getKey_(Node *__bst, int __key, Node **__out) {

    if (__bst->key == __key) {
        *__out = __bst;
        return;
    }

    if (__bst->right) getKey_(__bst->right, __key, __out);
    if (__bst->left ) getKey_(__bst->left , __key, __out);

}
// Return a pointer to the node that contains the key entered as an argument
Node *getKey(Node *__bst, int __key) {

    Node *out = NULL;

    getKey_(__bst, __key, &out);

    return out;
}


/**========================================================================
 *!                           Partie A
 *========================================================================**/
// I need a method to count the number of nodes in a binary tree.

int countNodes(const Node *__avl) {

    if (!__avl) return 0;

    return 1 + countNodes(__avl->left) + countNodes(__avl->right);
}

int numLeafs(const Node *__avl) {
    return countNodes(__avl) + 1;
}

Node *getNextKey(const Node *__bst) {

    Node *it = __bst->right;

    if (!it) return NULL;

    while (it->left) {
        it = it->left;
    }

    return it;

}

// start a dfs traversal.
void nextNode_(Node *__avl, int __key, Node **__sup) {

    // printf("|");

    if (!__avl) {
        // printf("Stopping iterating, __key: %d, __sup: %d", __key, (*__sup)->key);
        return;
    }


    // try a simple tree traversal, keeping note of the sup
    if (__avl->key <= (*__sup)->key &&  __avl->key > __key) {
        // printf("sup changed to %d\n", (*__sup)->key);
        *__sup = __avl;
    }

    // ok i've landed on the value. what to do?
    if (__avl->key == __key) {

        // printf("Found key %d!!\n", __key);
        Node *next = getNextKey(__avl);
        // if (__key == 47) printf("next: %d\n", next->key);
        // printf("Finished\n");
        if (next == NULL) { // then there are no more elements to the right of this node (like 7, 97 in arb)
            // printf("Next is null!\n");
            // printf("stopping iterating at %d\n", __avl->key);
            return; // stop iterating, we have already found the sup.
        } else {
            // printf("Sup set to %d\n", next->key);
            (*__sup) = next;
            // return;
            // nextNode_(__avl->right, __key, __sup);
        }
    }

    if (__avl->key > __key) {
        nextNode_(__avl->left, __key, __sup);
    } else {
        nextNode_(__avl->right, __key, __sup);
    }

}


Node *nextNode(Node *__avl, int __key) {

    if (!__avl) return NULL;

    Node *sup = __avl;

    // if we are starting from the root, set the sup to the element to the right
    if (__avl->key < __key) {

        // printf("key is smaller\n");

        if (__avl->right == NULL) return NULL; // theres nothing higher than the root then
        else sup = __avl->right;
    }
    // printf("Starting the with __key: %d at %d\n", __key, sup->key);

    nextNode_(__avl, __key, &sup);

    if (__key >= sup->key) return NULL;
    // if (__key == __avl->key) return NULL;
    // printf("======= sup:%d =======\n", sup->key);
    return sup;
}


Node *getMin(Node *__avl) {

    // traverse the tree as far left as possible
    if (!__avl) return NULL;

    Node *left_min = getMin(__avl->left);

    if (left_min == NULL) {
        return __avl;
    } else {
        return __avl->key < left_min->key ? __avl : left_min;
    }

}


int numChildren(const Node *__avl) {
    int count = 0;
    if (!__avl) return count;
    if (__avl->right) count ++;
    if (__avl->left) count ++;

    return count;
}

int countElementsWithEmpty(const Node *__avl) {

    if (!__avl) return 0;

    int this = numChildren(__avl) < 2 ? 1 : 0;

    return this + countElementsWithEmpty(__avl->left) + countElementsWithEmpty(__avl->right);

}

void extractNodes_(Node *__avl, Node ** __arr, int * __i) {

    if (!__avl) return;

    int nc = numChildren(__avl);
    if (nc < 2) {
        __arr[*__i] = __avl;
        (*__i)++;
    }

    extractNodes_(__avl->left, __arr, __i);
    extractNodes_(__avl->right, __arr, __i);
}

// Now actually go ahead and extract those nodes with empty spots
Node **extractNodesWithEmpty(Node *__avl) {

    int n = countElementsWithEmpty(__avl);

    Node **node_arr = (Node **) malloc(sizeof(Node *) * n);

    int i = 0;
    extractNodes_(__avl, node_arr, &i);

    return node_arr;
}

// I now have a method to iterate through the sorted elements
void toBalise(Node *__avl) {

    if (!__avl) return;

    int n = countNodes(__avl);
    int * keys = (int *) malloc(sizeof(int) * n);

    int i = 0;
    Node *it = getMin(__avl);
    // keys[0] = it->key;

    while (it) {

        keys[i] = it->key;
        it = nextNode(__avl, it->key);
        i ++;

    }

    printf("Gathered elements: \n");
    for (int i = 0; i < n; i++) {
        printf("%d, ", keys[i]);
    }
    printf("\n");

    // now we actually want to place those extra elements
    // it = getMin(__avl);

    // before I actually go ahead and add the new elements, I need to get an ARRAY
    // of the elements with AT LEAST ONE empty descendant
    int n_nodes = countNodes(__avl);
    int n_empty = countElementsWithEmpty(__avl);
    Node **node_array = extractNodesWithEmpty(__avl);
    Node *this = NULL;

    for (int i = 0, j = 0; i < n_empty; i++) {

        this = node_array[i];
        // printf("visiting %d\n", this->key);

        while (numChildren(this) < 2 && j < n_nodes) {
            // add the elements...
            if (this->left == NULL) {
                printf("key[%d]: %d\n", j, keys[j]);
                // this->left = newNode(keys[j] * 1000);
                this->left = newNode(keys[j]);
                j++;
            } else {
                printf("key[%d]: %d\n", j, keys[j]);
                // this->right = newNode(keys[j] * 1000);
                this->right = newNode(keys[j]);
                j++;
            }
        }
    }


    // while (it) {

    //     // while it has space, add the next node...
    //     while (numChildren(it) < 2) {
    //         if (!it->left) {
    //             printf("Adding %d to %d\n", keys[i], it->key);
    //             it->left = newNode(keys[i] + 2000);
    //             i ++;
    //         } else {
    //             printf("Adding %d to %d\n", keys[i], it->key);
    //             it->right = newNode(keys[i] + 2000);
    //             i ++;
    //         }
    //     }

    //     it = nextNode(__avl, it->key);

    // }

}

void getLeafBalise_(Node *__bal, int __key, Node **__out) {

    if (!__bal) return;

    if (isLeaf(__bal) && __key == __bal->key) {

        *__out = __bal;
        return;

    }

    // traverse along, keeping the non null value
    if (__key > __bal->key) {
        getLeafBalise_(__bal->right, __key, __out);
    } else {
        getLeafBalise_(__bal->left, __key, __out);
    }
}

// Get a leaf of a balise tree that matches the __key;
Node *getLeafBalise(Node *__bal, int __key) {

    if (!__bal) return NULL;

    Node *out = NULL;
    getLeafBalise_(__bal, __key, &out);
    return out;

}

Node *insertBalise(Node *__bal, int __key) {

    if (!__bal) return newNode(__key);

    if (isLeaf(__bal)) {
        // proceed with insert
        if (__key < __bal->key) {
            // then place it on the left side
            __bal->left = newNode(__key);
            __bal->right = newNode(__bal->key); // duplicate this one
        } else {
            __bal->right = newNode(__key);
            __bal->left = newNode(__bal->key);
        }
    } else {
        // scurry along to the next Node
        __key <= __bal->key ? insertBalise(__bal->left, __key) : insertBalise(__bal->right, __key);
    }

    return __bal;
}

void getParentBalise_(Node *__bal, int __key, Node **__out) {

    if (__bal == NULL) return;
    if (isLeaf(__bal)) return;

    if (__key <= __bal->key) {
        // go left
        if (__bal->left->key == __key && isLeaf(__bal->left)) {
            // we've found the parent
            *__out = __bal;
            return;
        } else {
            getParentBalise_(__bal->left, __key, __out);
        }
    } else { // go right
        if (__bal->right->key == __key && isLeaf(__bal->right)) {
            *__out = __bal;
            return;
        } else {
            getParentBalise_(__bal->right, __key, __out);
        }
    }
}

Node *getParentBalise(Node *__bal, int __key) {

    Node *out = NULL;
    getParentBalise_(__bal, __key, &out);

    return out;
}


// return the new root
Node *deleteBalise(Node *__bal, int __key) {

    if (__bal == NULL) return NULL;

    // locate parent.
    Node *parent = getParentBalise(__bal, __key);
    if (parent == NULL) return __bal; // nothing to remove

    if (parent == __bal) {
        // then we are dealing with the root
        if (__bal->left->key == __key) {
            Node *temp = __bal->right;
            __bal->right = NULL; // THIS introduces a memory leak!!!
            __bal->left = NULL;
            return temp;
        }

        if (__bal->right->key == __key) {
            Node *temp = __bal->left;
            __bal->right = NULL;
            __bal->left = NULL;
            return temp;
        }

    } else {
        // we arent dealing with the root and we can return

        if (parent->left->key == __key) {
            parent->key = parent->right->key;
            parent->left = NULL;
            parent->right = NULL;
        } else if (parent->right->key == __key) {
            // both the right sides are the same
            parent->key = parent->left->key;
            parent->left = NULL;
            parent->right = NULL;

        } else {

            parent->left = NULL;
            parent->right = NULL;
        }

    }
    return __bal;

}


// ASSUME that we have a balanced balise tree already. Then we know any element that we are adding
// to any leaf has two empty spots. Simply add the new one and duplicate the parent
// that we are adding to.
// Node *insertBalise(Node *__bal, int __key) {

    // First thing we have to do is traverse the tree and find the appropriate spot.
    //

    // Am I the root?
    // Node *leaf = getLeafBalise(__bal, )



// }




/**========================================================================
 *!                           Main stuff
 *========================================================================**/

void t_bin();
void t_avl_left_chain();
void t_avl_right_chain();
void partie_a();
void t_balise();

int main() {


    // t_bin();
    // t_avl_left_chain();
    // t_avl_right_chain();

    partie_a();
    t_balise();

    return 0;
}

void t_balise() {

    Node *bal = newNode(7);
    insertBalise(bal, 8);
    insertBalise(bal, 10);

    printf(" ================ t_balise ================\n");
    createDotBST(bal, "bal.dot");
    updateBST(bal);

    printHeights(bal);

    Node *root = getParentBalise(bal, 7);
    Node *eight = getParentBalise(bal, 8);
    Node *ten = getParentBalise(bal, 10);

    printf("root: %d\n", root->key);
    printf("father of 8: %d\n", eight->key);
    printf("father of 10: %d\n", ten->key);

    printf("ten == eight %d\n", ten == eight);

    bal = deleteBalise(bal, 8);

    // createDotBST(bal, "bal_del7.dot");
    createDotBST(bal, "bal_del8.dot");

}

void partie_a() {

    Node *arb = newNode(47);

    addKeyBST(arb, 80);
    addKeyBST(arb, 89);
    addKeyBST(arb, 97);
    addKeyBST(arb, 62);
    addKeyBST(arb, 59);
    addKeyBST(arb, 70);
    addKeyBST(arb, 25);
    addKeyBST(arb, 10);
    addKeyBST(arb, 37);
    addKeyBST(arb, 7);
    addKeyBST(arb, 19);
    addKeyBST(arb, 30);

    createDotBST(arb, "arb.dot");

    printf("n nodes: %d\n", countNodes(arb));

    int n = countNodes(arb);

    Node *ten = getKey(arb, 10);

    printf("numchildren(%d) = %d\n", ten->key, numChildren(ten));
    printf("num with empty spots: %d\n", countElementsWithEmpty(arb));

    int nempty = countElementsWithEmpty(arb);

    Node **node_arr = extractNodesWithEmpty(arb);

    for (int i = 0; i < nempty; i++) {
        printf("Stored: %d\n", (node_arr[i])->key);
    }

    toBalise(arb);

    createDotBST(arb, "bal.dot");

    Node *test = getLeafBalise(arb, 7);

    printf("numchildren(%d) -> %d\n", test->key, numChildren(test));
    printHeights(test);

    printf("====\n");

    updateBST(arb);

    printHeights(arb);

    // Node *test_root = getLeafBalise(arb, 47);


    // printf("It: %d\n", it->key);



    // printf("after 97 comes: %d\n", next->key);

}






void t_bin() {

    Node *avl = newNode(10);

    addKeyBST(avl, 14);
    addKeyBST(avl, 8);
    addKeyBST(avl, 9);
    addKeyBST(avl, 4);
    addKeyBST(avl, 3);

    createDotBST(avl, "test.dot");

    avl = rotRight(avl, 10);

    createDotBST(avl, "test_rotr10.dot");

    avl = rotLeft(avl, 8);

    createDotBST(avl, "test_rot.dot");

    avl = rotLeft(avl, 4);
    avl = rotRight(avl, 4);

    createDotBST(avl, "test_rot4r.dot");

    updateBST(avl);
    printHeights(avl);

    printf("Height avl: %d\n", avl->height);

    updateBST(avl);
    printHeights(avl);

    addKeyBST(avl, 15);
    addKeyBST(avl, 16);
    addKeyBST(avl, 17);
    addKeyBST(avl, 18);

    createDotBST(avl, "test_more.dot");

    updateBST(avl);

    printf("=========\n");
    printHeights(avl);
}

void t_heights() {

}


Node *test_add(Node *avl, int key) {
    avl = addKeyAVL(avl, key);
    assert(isBalanced(avl));
    return avl;
}

void t_avl_left_chain() {

    printf("============ t_avl =============\n");
    Node *avl = newNode(10);

    avl = test_add(avl, 8);
    avl = test_add(avl, 6);
    avl = test_add(avl, 4);
    avl = test_add(avl, 3);
    avl = test_add(avl, 2);
    avl = test_add(avl, 1);

    printHeights(avl);

    createDotBST(avl, "avl_left.dot");
}

void t_avl_right_chain() {

    printf("============ t_avl =============\n");
    Node *avl = newNode(10);



    avl = addKeyAVL(avl, 11);
    assert(isBalanced(avl));
    avl = addKeyAVL(avl, 12);
    assert(isBalanced(avl));
    avl = addKeyAVL(avl, 13);
    assert(isBalanced(avl));
    avl = addKeyAVL(avl, 14);



    createDotBST(avl, "error.dot");

    printHeights(avl);

    updateBST(avl);
    printHeights(avl);



    // assert(isBalanced(avl));
    avl = addKeyAVL(avl, 15);
    // assert(isBalanced(avl));
    avl = addKeyAVL(avl, 16);

    createDotBST(avl, "avl_right.dot");

    printHeights(avl);

}


