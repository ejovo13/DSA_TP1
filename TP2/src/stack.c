#include "stack.h"

Stack *newStack(int __vertex) {

    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->prev = NULL;
    stack->vertex = __vertex;

    return stack;
}

// Push a value to the top of the stack and redundantly return a pointer to
// the final position
void pushStack(Stack **__stack, int __vertex) {

    if (!*__stack) {
        *__stack = newStack(__vertex);
        return;
    }

    Stack *stack = newStack(__vertex);
    stack->prev = *__stack;

    *__stack = stack;
}

// Pop the last element in the stack
// If the stack is empty, return -1.
// ***HOWEVER***, I want the caller to ALWAYS
// check whether or not the stack is empty.
// This is because I'd rather check for a null pointer than a -1
int popStack(Stack **__stack) {

    if (!*__stack) return -1;

    int temp = (*__stack)->vertex;
    *__stack = (*__stack)->prev;

    return temp;
}

void printStack(Stack *__stack) {

    if (!__stack) {
        printf("Stack empty\n");
        return;
    }

    Stack *it = __stack;

    printf("%d", it->vertex);

    while (it->prev) {

        printf(" -> %d", it->prev->vertex);
        it = it->prev;
    }

    printf("\n");
}