# Stack

We can use the `Stack` structure as a LIFO container that is used to implement BFS and routines to determine strongly connected components.

To instantiate a `Stack`, we use the function `newStack` to return a pointer to newly allocated `Stack` that currently holds no`Vertex`

### Construction

```{C}
Stack *s = newStack();
```

### Push/Pop

To Add vertices to the stack, we simple call the companion functions `pushStack`/`popStack`. Following conventions, pushing to the stack will add a new element on top whereas popping the stack will remove the top element from the stack and then return that element.

```
pushStack(s, 10);
pushStack(s, 40);
pushStack(s, 13);

Vertex v0 = popStack(s);
Vertex v1 = popStack(s);
Vertex v2 = popStack(s);
```

### Freeing Memory

When it comes to freeing the memory that has been allocated on subsequent calls to `pushStack`, we can use handy utility function `freeStack`. This routine will remove all of the `Vertex` objects that currently exist in the stack, freeing them as they are released.

```
pushStack(s, 1);
pushStack(s, 2);
pushStack(s, 3);

printf("Size of stack %d\n", stackSize(s)); // 3

freeStack(s);

printf("Size of stack %d\n", stackSize(s)); // 0

pushStack(s, 10);
```

This routine is useful when we want a fresh start with the stack and we don't want any memory leaks in our program. If you no longer wish to use the `Stack *s`, consider calling the `releaseStack` function to also clean up the space allocated by `newStack` and to nullify the pointer s.

```
releaseStack(&s) // s freed and set to NULL here
```

`releaseStack` will free all of the nodes populating the stack and also release the memory pointed to by `s`. This shall be called when your program no longer needs to use the `Stack`.

### Inquiry

We can inspect the state of our `Stack` using the utility functions `stackSize` and `printStack`.
