#pragma once

#include <stdlib.h>

#include "list.h"

// As a coding exercise, I'd like to also implement a BFS (Breadth-first search)
// In order to do so, I need a "Queue" object. This is a FIFO data structure that can
// be modelled with two pointers.

typedef struct {

    Vertex first;
    Vertex last;

} Queue;

Queue *newQueue();

// Add the value __val to the queue
void enQueue(Queue *__q, GRAPH_TYPE __val);

Vertex deQueue(Queue *__q);

void printQueue(Queue *__q);

// Now that I've got a working queue, I can actually implement BFS Search!!!
void queueInfo(Queue *__q);

// If queue is empty, return 0.
// If the pointer __q is null, return -1
int queueSize(Queue *__q);

// Free the contents of a queue.
void freeQueue(Queue *__q);

// Free the memory that points to this Queue object itself, set the pointer to NULL,
// and free all of the elements in the queue
void releaseQueue(Queue **__q);