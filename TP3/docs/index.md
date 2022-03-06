# TP3 Binary Search Trees

This documentation shows the usage of the functions that are used in order as they appear in the TP3 document.

### Part A
##### BinTree Structure

For a more information, consult [BinTree](./bintree.md)

##### Insertion
To insert an element, simply call `addKeyBST`

```
BinTree *root = newBinTree(1);
addKeyBST(root, 2);
addKeyBST(root, 3);
```

##### Deletion
To delete an element, simply call `removeKeyBST`

```
removeKeyBST(root, 2);
```

##### Freeing Memory
To delete all of the nodes that exist in a `BinTree`, call `releaseTree` on the address of a `BinTree` to nullify the pointer and to free all of the nodes that were manually allocated.

```
releaseTree(&root); // root set to null
```

##### Disequilibrium Factor
We can measure the "unevenness" of a graph by comparing the heights of the left and right subtrees of the root. The disequilibrium factor is thus the positive difference between those two values. Therefore a graph with a disequilibrium factor of 0 has two subgraphs that are the same height.

```
heightBST(root);
deqFactor(root);
```

##### Printing

To "print" the graph, use the `createDotBST` function along with the name of the output file:

```
createDotBST(root, "output.dot");
```

### Part B
In part B we study how the average height and average disequilibrium factor vary as a function of the size of the graph.

Using the `range` function as defined in the `ejovo` library will give me a range of different sizes between `10` and `maxSize` that I can iterate through to store the average heights and disequilibrium factor.

We start off by creating a random graph with the `createRandomTree` function. This procedure generates a Fischer-Yates shuffle of the elements `{1, ..., N}` and then adds those elements to a newly constructed `BinTree`.

We use the `heightBST` function to recursively compute the height and the `deqFactor` function to gather the difference between the heights of the right and left subgraphs.