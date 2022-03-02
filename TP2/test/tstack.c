#include "stack.h"

#include <stdio.h>

int main() {

    printf("Testing stacks...\n");

    Stack *s = newStack();

    pushStack(s, 8);
    pushStack(s, 23);

    printStack(s);

    Vertex tmp = popStack(s);

    printf("Popping... %d\n", tmp->data);

    free(tmp);

    printStack(s);

    pushStack(s, 100);
    pushStack(s, 18);
    pushStack(s, -99);

    printStack(s);

    free(popStack(s));
    free(popStack(s));
    free(popStack(s));
    free(popStack(s));

    printStack(s);

    releaseStack(&s);

    return 0;
}