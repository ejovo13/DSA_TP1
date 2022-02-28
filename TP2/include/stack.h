#pragma once

#include <stdio.h>
#include <stdlib.h>

// Various data structures that will help me implement the exercices for TP2-graphes.

// STRONGLY CONNECTED COMPONENTS
// To find out whether or not a component is strongly connected, we will implement
// Kosaraju's algorithm
// En bref, this consists of performing two DFS searches (the second one on a reversed graph)
// and keeping track of already visited vertices in a stack.

// In order to keep track of my vertices in a stack, I first need to create a stack object.

// A stack is simply a LIFO data type that can be modelled with a linked list.
// that is linked "backwards" instead of forwards. Conceptually, I'd like to use a
// "prev" field rather than a "next field"
typedef struct stack_t {

    int vertex;
    struct stack_t *prev;

} Stack;

Stack *newStack(int __vertex) {

    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->prev = NULL;
    stack->vertex = __vertex;

    return stack;
}

// Push a value to the top of the stack and redundantly return a pointer to
// the final position
Stack *pushStack(Stack *__stack, int __vertex) {

    if (!__stack) return newStack(__vertex);

    Stack *stack = newStack(__vertex);
    stack->prev = __stack;

    return stack;
}

// Pop the last element in the stack
// If the stack is empty, return -1.
// ***HOWEVER***, I want the caller to ALWAYS
// check whether or not the stack is empty.
// This is because I'd rather check for a null pointer than a -1
int popStack(Stack *__stack) {

    if (!__stack) return -1;

    int temp = __stack->vertex;
    __stack = __stack->prev;

    return temp;
}

void printStack(Stack *__stack) {

    if (!__stack) printf("Stack empty\n");

    Stack *it = __stack;

    printf("%d ", it->vertex);

    while (it->prev) {

        printf("-> %d", it->prev->vertex);
        it = it->prev;
    }
}