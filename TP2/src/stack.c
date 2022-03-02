#include "stack.h"

Stack *newStack() {

    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

// Push a value to the top of the stack and redundantly return a pointer to
// the final position
void pushStack(Stack *__stack, int __vertex) {

    if (!__stack->top) {
        __stack->top = newNode(__vertex);
        return;
    }

    Vertex v = newNode(__vertex);

    v->next = __stack->top;
    __stack->top = v;

}

// Pop the last element in the stack
// If the stack is empty, return -1.
// ***HOWEVER***, I want the caller to ALWAYS
// check whether or not the stack is empty.
// This is because I'd rather check for a null pointer than a -1
Vertex popStack(Stack *__stack) {

    if (!__stack->top) return NULL;

    Vertex temp = __stack->top;
    __stack->top = temp->next;

    return temp;
}

void printStack(Stack *__stack) {

    if (!__stack->top) {
        printf("Stack empty\n");
        return;
    }

    Vertex it = __stack->top;
    printf("%d", it->data);

    while (it->next) {

        printf(" -> %d", it->next->data);
        it = it->next;
    }

    printf("\n");
}

// Return 0 is __stack is null
int stackSize(Stack *__stack) {

    if (!__stack) return 0;

    int count = 0;

    Vertex it = __stack->top;

    while(it) {

        count++;

        it = it->next;
    }

    return count;
}

// Free all of the elements that exist in a stack. This will NOT free the
// memory of the POINTER __stack. This will only free the memory that is pointed
// to by the nodes (vertices) of the __stack, starting from __stack->top
void freeStack(Stack *__stack) {

    Vertex it = __stack->top;
    Vertex temp;

    while (it) {

        temp = it;
        it = it->next;
        free(temp);

    }

    __stack->top = NULL;
}

// Free the nodes of the stack, free the memory pointed to by a stack pointer
// and then set that pointer to NULL so that we never try to access the memory again
void releaseStack(Stack **__stack) {

    if (!__stack) return; // This is kinda redundant right?
    if (!*__stack) return; // If the stack isn't pointing to anything, do nothing

    freeStack(*__stack);
    free(*__stack);

    *__stack = NULL;
}