#include <assert.h>

#include "queue.h"

int main() {

    // Create a new queue.

    Queue *q = newQueue();

    enQueue(q, 10);
    enQueue(q, 3);
    enQueue(q, -3);
    enQueue(q, 15);

    assert(queueSize(q) == 4);

    Vertex tmp = deQueue(q);

    assert(tmp->data == 10);
    free(tmp);

    tmp = deQueue(q);
    assert(tmp->data == 3);

    free(tmp);

    assert(queueSize(q) == 2);

    enQueue(q, 100);
    assert(queueSize(q) == 3);

    tmp = deQueue(q);
    assert(tmp->data == -3);
    free(tmp);

    tmp = deQueue(q);
    assert(tmp->data == 15);

    free(tmp);

    tmp = deQueue(q);
    assert(tmp->data == 100);
    free(tmp);

    releaseQueue(&q);
    assert(!q);

    printf("All checks validated, queue working properly\n");

    return 0;
}