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
/**
 * @brief Implementation of a Last in, first out (LIFO) stack data structure
 * @file
 *
 */

/**
 * @brief A Simple LIFO data structure implementation
 *
 */
typedef struct {

    Vertex top;

} Stack;

// Simply create a new stack that we can add elements to
/**
 * @brief Allocate a new, EMPTY stack
 *
 * newStack() serves as the constructor of a Stack object. The created stack has a default NULL
 * top pointer.
 *
 * @return A pointer to a malloc'ed object
 */
Stack *newStack();

// Push a value to the top of the stack and change the element that the
// top pointer is pointing at
/**
 * @brief Create a new Vertex object (Node) and push it to the stack
 *
 * @param __stack The stack to modify
 * @param __vertex The integer value of the vertex that we would like to push
 */
void pushStack(Stack *__stack, int __vertex);

// Pop the last element in the stack
// If the stack is empty, return -1.
// ***HOWEVER***, I want the caller to ALWAYS
// check whether or not the stack is empty.
// This is because I'd rather check for a null pointer than a -1
/**
 * @brief Pop the top Vertex from the stack __stack and reassign __stack's top pointer
 *
 * @param __stack
 * @return Vertex
 */
Vertex popStack(Stack *__stack);

/**
 * @brief Print the contents of __stack
 *
 * @param __stack
 */
void printStack(Stack *__stack);

/**
 * @brief Count the number of Vertices that reside in the Stack
 *
 * @param __stack
 * @return int
 */
int stackSize(Stack *__stack);

// Free all of the elements that exist in a stack. This will NOT free the
// memory of the POINTER __stack. This will only free the memory that is pointed
// to by the nodes (vertices) of the __stack, starting from __stack->top
/**
 * @brief Free all of the Vertices that exist in the stack
 *
 * @param __stack
 */
void freeStack(Stack *__stack);

// Free the nodes of the stack, free the memory pointed to by a stack pointer
// and then set that pointer to NULL so that we never try to access the memory again
/**
 * @brief Free all of the vertices that exist in the stack AND release the memory pointed to by *__stack
 *
 * @param __stack
 */
void releaseStack(Stack **__stack);