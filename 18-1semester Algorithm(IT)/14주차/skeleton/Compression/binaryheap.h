#ifndef COMPRESSION_BINARYHEAP_H
#define COMPRESSION_BINARYHEAP_H

#include "run.h"
typedef Run *Item;
typedef int (*CompareFtnType) (Item, Item);

typedef struct heap {
    Run**data;
    int heapSize;
    int capacity;
    CompareFtnType compareFtn;
} Heap;

Heap *create_heap( CompareFtnType f, int capacity );
bool insert(Heap *, Item);
Item extractMin(Heap *);

#endif //COMPRESSION_BINARYHEAP_H
