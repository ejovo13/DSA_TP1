#include "stack.h"

Stack *newStack(int __vertex) {

    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

// Push a value to the top of the stack and redundantly return a pointer to
// the final position
void pushStack(Stack *__stack, int __vertex) {

    if (!__stack) {
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

    if (!__stack) {
        printf("Stack empty\n");
        return;
    }

    Vertex it = __stack->top;

    if (it) printf("%d", it->data);

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