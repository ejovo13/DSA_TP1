# Stack

We can use the `Stack` structure as a LIFO container that is used to implement BFS and routines to determine strongly connected components.

To instantiate a `Stack`, we use the function `newStack` to return a pointer to newly allocated `Stack` that currently holds no`Vertex`

```{C}
Stack *s = newStack();
```

To Add vertices to the stack, we simple call the companion functions `pushStack`/`popStack`. Following conventions, pushing to the stack will add a new element on top whereas popping the stack will remove the top element from the stack and then return that element.

```
pushStack(s, 10);
pushStack(s, 40);
pushStack(s, 13);

Vertex v0 = popStack(s);
Vertex v1 = popStack(s);
Vertex v2 = popStack(s);
```

