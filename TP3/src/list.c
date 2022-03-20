// Make _another_ node
#include "list.h"

Node *newNode(int __val) {

    Node *node = (Node *) malloc(sizeof(Node));
    node->val = __val;
    node->next = NULL;

    return node;
}

List *newList() {

    List *list = (List *) malloc(sizeof(List));
    list->head = NULL;
    list->last = NULL;

    return list;
}

List *appendList(List *__list, int __val) {

    // Null edge guard
    if (__list == NULL) {
        // printf("List empty, creating new list\n");
        List *list = newList();
        list->head = newNode(__val);
        list->last = list->head;
        return list;
    }

    // if the head is null
    if (__list->head == NULL) {
        // printf("Head is empty, adding %d to front\n", __val);
        __list->head = newNode(__val);
        __list->last = __list->head;
        return __list;
    }

    // otherwise let's just add directly to the end!
    if (__list->last == NULL) {
        // printf("Last is null, modifying __list and then appending to the end")
        // This statement is only reached if the head is NOT null. Thus we are in an "undefined"
        // state where we supposedly have a head but not a final node. So, set the last to the head
        // and recall appendList.
        __list->last = __list->head;
        return appendList(__list, __val);
    }

    __list->last->next = newNode(__val);
    __list->last = __list->last->next;
    return __list;
}

size_t lenList(const List *__list) {
    return lenNode(__list->head);
}

Node *atList(const List *__list, int __index) {
    return atNode(__list->head, __index);
}

Node *penultimateNode(const Node *__node) {
    Node *it = __node;

    if (it == NULL) return NULL;
    if (it->next == NULL) return NULL;

    while (it->next->next) {
        it = it->next;
    }

    return it;

}

Node *secondToLast(const List *__list) {

    // Find the node that is second to last.
    if (__list == NULL) return NULL; // list has
    if (__list->head == NULL) return NULL; // list has zero elements
    if (__list->head == __list->last) return NULL; // list has one element
    if (__list->last == NULL) return NULL;

    // Iterate along the lists until we find the last element
    return penultimateNode(__list->head);
}

// Extract an element from the list given a certain index,
// REMOVING the element from the linked list
Node *extractNode(List *__list, int __index) {

    if (__list == NULL) return NULL;
    if (__list->head == NULL) return NULL;

    Node *rem = atList(__list, __index);

    return extractNodePtr(__list, rem);
}

Node *extractNodePtr(List *__list, Node *__rem) {

    // is this the head?
    if (__list->head == __rem) {
        // removeNode();

        // If the list only has one element, then nullify them both
        if (__rem == __list->last) {
            __list->head = NULL;
            __list->last = NULL;

            __rem->next = NULL;
            return __rem;
        }

        // otherwise just sever the bottom off, not messing with the last pointer
        __list->head = __rem->next;
        __rem->next = NULL;
        return __rem;
    }

    // is this the last element?
    if (__list->last == __rem) {

        // then let's get the penultimate element
        Node *penult = secondToLast(__list);

        // and cut ties
        __list->last = penult;
        penult->next = NULL;

        return __rem;
    }

    // otherwise it's an interior node, call removeNode
    return removeNodePtr(__list->head, __rem);

}

// Free the list and then return a null pointer.
// thus we can call some thing like
//
// list = freeList(list);
//
// Alternatively, use the function releaseList(**List) to reset the
// pointer to NULL:
//
// releaseList(&list);
List *freeList(List *__list) {
    freeNode(__list->head);
    return NULL;
}

// release and reset to NULL
void releaseList(List **__plist) {
    if (!__plist) return;
    freeList(*__plist);
    *__plist = NULL;
}

// append element to end of the node
// and return a pointer to the node NEWLY_CREATED_ELEMENT. If __node
// is originally null, we HAVE to reassign it when calling
// appendNode. Therefore, it is best practice to call
// node = appendNode(node, 10).
Node *appendNode(Node *__node, int __val) {

    // If the node is null, create a new node node
    if (!__node) return newNode(__val);

    // Now let's proceed iterativeley
    Node *it = __node;
    while (it->next) {
        it = it->next;
    }

    Node *next = newNode(__val);

    it->next = next;
    return next;
}

// Free all of the node's elements and set __node to NULL
void releaseNode(Node **__pnode) {

    if (!__pnode) return;

    freeNode(*__pnode);

    *__pnode = NULL;
}

// recursively free all of the nodes in the node
void freeNode(Node *__node) {

    if (!__node) return;

    freeNode(__node->next);
    free(__node);
}

size_t lenNode(const Node *__node) {

    if (!__node) return 0;

    return 1 + lenNode(__node->next);
}

Node *atNode(const Node *__node, int __index) {

    if (__node == NULL) return NULL;
    if (__index < 0)    return NULL;
    if (__index == 0)   return __node;
    return atNode(__node->next, __index - 1);
}

// Remove the node and then return the removed node pointer.
// return NULL if we couldn't find it
Node *removeNode(Node *__node, int __val) {

    // handle the first case
    if (__node == NULL) return NULL;
    if (__node->val == __val) {
        __node->next = NULL; // sever the tail of the root node
        return __node;
    }

    Node *it = __node;
    Node *rem = NULL;

    while (it->next) {

        if (it->next->val == __val) {
            // Then next is the node we want to remove.
            rem = it->next;

            it->next = rem->next; // create the connection between before and after
            rem->next = NULL;
            return rem;
        }

        it = it->next;
    }

    // If I didn't find the node to remove
    return NULL;
}

Node *removeNodePtr(Node *__node, Node *__rem) {

    if (__node == NULL) return NULL;
    if (__node == __rem) {
        __node->next = NULL; // sever the tail of the root node
        return __node;
    }

    Node *it = __node;
    // Node *rem = NULL;

    while (it->next) {

        if (it->next == __rem) {
            // Then next is the node we want to remove.
            // rem = it->next;

            it->next = __rem->next; // create the connection between before and after
            __rem->next = NULL;
            return __rem;
        }

        it = it->next;
    }

    // If I didn't find the node to remove
    return NULL;

}

void printList(const List *__list) {

    Node *it = __list->head;

    printf("{");

    if (it == NULL) {
        printf("}\n");
        return;
    }

    while (it->next) {

        printf("%d, ", it->val);
        it = it->next;
    }

    printf("%d}\n", it->val);
}

// Return the pointer to the node who has the val __val, and NULL
// if it doesnt exist.
Node *getNode(const List *__list, int __val) {

    if (__list == NULL) return NULL;

    Node *it = __list->head;

    while (it) {

        if (it->val == __val) return it;

        it = it->next;
    }

    return it;
}

// check if __val appartient a __list
bool isElementList(const List *__list, int __val) {
    return getNode(__list, __val) != NULL;
}