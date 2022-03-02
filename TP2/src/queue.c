#include "queue.h"

Queue *newQueue() {

    Queue *q = (Queue *) malloc(sizeof(Queue));
    q->first = NULL;
    q->last = NULL;

    return q;

}

// Add the value __val to the queue
void enQueue(Queue *__q, GRAPH_TYPE __val) {

    // If nobody is in line, add this one as the first and last pointer
    if (!__q->first) {
        __q->first = newNode(__val);
        __q->last = __q->first;
    } else { // Add this node to the end of the line
        __q->last->next = newNode(__val);
        __q->last = __q->last->next;
    }
}

Vertex deQueue(Queue *__q) {

    if (!__q->first) return NULL; // If line is empty

    Vertex temp = __q->first;
    __q->first = __q->first->next;

    return temp;
}

void printQueue(Queue *__q) {

    if (!__q) return;

    Vertex it = __q->first;

    printf("Queue: {");

    while (it) {
        printf("%d, ", it->data);
        it = it->next;
    }

    printf("}\n");
}

// Now that I've got a working queue, I can actually implement BFS Search!!!
void queueInfo(Queue *__q) {
    if (!__q) return;

    printf("First: %x, Last: %x ", __q->first, __q->last);
    if (__q->first) printf("first->data: %d, ", __q->first->data);
    if (__q->last) printf("last->data: %d", __q->last->data);
    printf("\n");
}

// If queue is empty, return 0.
// If the pointer __q is null, return -1
int queueSize(Queue *__q) {

    if (!__q) return -1;

    int count = 0;

    Vertex it = __q->first;

    while (it) {
        count ++;
        it = it->next;
    }

    return count;
}

// Free the contents of a queue.
void freeQueue(Queue *__q) {

    Vertex it = __q->first;
    Vertex temp;

    while (it) {

        temp = it;
        it = it->next;
        free(temp);

    }

    __q->first = NULL;
    __q->last  = NULL;
}

// Free the memory that points to this Queue object itself, set the pointer to NULL,
// and free all of the elements in the queue
void releaseQueue(Queue **__q) {

    if (!__q) return;
    if (!*__q) return;

    freeQueue(*__q);
    free(*__q);

    *__q = NULL;
}