#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

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

    Vertex top;

} Stack;

// Simply create a new stack that we can add elements to
Stack *newStack();

// Push a value to the top of the stack and change the element that the
// top pointer is pointing at
void pushStack(Stack *__stack, int __vertex);

// Pop the last element in the stack
// If the stack is empty, return -1.
// ***HOWEVER***, I want the caller to ALWAYS
// check whether or not the stack is empty.
// This is because I'd rather check for a null pointer than a -1
Vertex popStack(Stack *__stack);

void printStack(Stack *__stack);

int stackSize(Stack *__stack);