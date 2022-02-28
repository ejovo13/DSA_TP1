#pragma once

#include "list.h"
#include "queue.h"
#include "stack.h"

typedef struct {

    AdjacencyList adj;
    int nedge;
    int nvert;
    bool di;

} Graph;