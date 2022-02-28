#include "stack.h"

#include <stdio.h>

int main() {

    printf("Testing stacks...\n");

    Stack *s = newStack(10);

    pushStack(&s, 8);
    pushStack(&s, 23);

    printStack(s);

    printf("Popping... %d\n", popStack(&s));

    printStack(s);

    pushStack(&s, 100);
    pushStack(&s, 18);
    pushStack(&s, -99);

    printStack(s);

    popStack(&s);
    popStack(&s);
    popStack(&s);
    popStack(&s);
    popStack(&s);
    popStack(&s);
    popStack(&s);
    popStack(&s);
    popStack(&s);

    printStack(s);

    return 0;
}