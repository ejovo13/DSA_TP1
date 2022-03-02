#include "queue.h"
#include <assert.h>

int main() {

    // Create a new queue.

    Queue *q = newQueue();

    enQueue(q, 10);
    enQueue(q, 3);
    enQueue(q, -3);
    enQueue(q, 15);

    assert(queueSize(q) == 4);

    assert(deQueue(q)->data == 10);
    assert(deQueue(q)->data == 3);

    assert(queueSize(q) == 2);

    enQueue(q, 100);
    assert(queueSize(q) == 3);

    assert(deQueue(q)->data == -3);
    assert(deQueue(q)->data == 15);
    assert(deQueue(q)->data == 100);

    printf("All checks validated, queue working properly\n");

    return 0;
}