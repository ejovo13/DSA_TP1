# Node / Vertex

A `Node` is the implementation of a Linked List data structure whose data types are integers. That is, every `Node` has a (possibly null) pointer pointing to the next `Node` and also a `data` field of type integer.

```
typedef struct node_t {

    int data;
    struct node_t *next;

} Node

typedef (Node *) Vertex;
```



Used throughout this project is the typedef `Vertex -> Node *` so that I can more easily reason about the algorithms used to traverse graphs. Thus, in the example codes we will see a lot of `Vertex`s rather than `Node *`.

### Instantiation

To create a new linked list, simply use the function `newNode` constructor

```
Vertex v1 = newNode(1);
Vertex v2 = newNode(2);
Vertex v3 = newNode(3);
Vertex v4 = newNode(4);

v3->next = v2;
v2->next = v4;
v4->next = v1;
```

This example creates a list whose nodes data is connected as: `3 -> 2 -> 4 -> 1`

We can duplicate the contents of a list with `duplicateList`, which returns a list whose nodes have been newly allocated.

```
Vertex list2 = duplicateList(v3);
```

### Memory

The individual vertices that reside in a linked list can be cleared with the function `freeList`. Individual nodes can be freed with `freeNode`.

It turns out that there are actually no functions to add another `Node` to an existing linked list as all that higher-level behavior is incorporated in the [Graph](graph.md) algorithms like `addEdge`.



The main functionality described by `list.h` (which declares the `Node` and `Vertex` types) are utility functions that are directly called by other examples. For the routine `writeVertex` is used when writing a `Graph` as a dot file. `countUnvisitedVertex` is a special function that takes a list of vertices in as an argument along with a boolean array of previously visited vertices to determine the number of unvisited vertices residing in the list.