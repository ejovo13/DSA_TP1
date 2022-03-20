#include "list.h"
#include <assert.h>

int main() {

    List *list = newList();

    appendList(list, 0);
    appendList(list, 1);
    appendList(list, 2);
    appendList(list, 3);
    appendList(list, 4);
    appendList(list, 5);

    printList(list);

    printf("Node at i: 3 --- %d\n", atList(list, 3)->val);
    printf("Node at i: 4 --- %d\n", atList(list, 4)->val);
    printf("Node at i: 0 --- %d\n", atList(list, 0)->val);

    assert(atList(list, -2) == NULL);
    assert(atList(list, 6) == NULL);

    Node *sec = secondToLast(list);

    printf("penultimate: %d\n", sec->val);

    assert(secondToLast(list)->val == 4);

    Node *head = extractNode(list, 0);

    assert(head->next == NULL);
    assert(head->val == 0);

    printList(list);

    Node *tail = extractNodePtr(list, list->last);

    assert(tail->val == 5);

    printList(list);


    assert(getNode(list, 100) == NULL);
    assert(getNode(list, 3)->val == 3);

    Node *three = getNode(list, 3);

    extractNodePtr(list, three);

    printList(list);

    assert(lenList(list) == 3);


    return 0;
}