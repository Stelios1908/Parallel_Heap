# Parallel_Heap
parallel heap 

a program in which a process in Linux/Unix spawns another N of its children.

Each process i (1<=i<=N), reads the value stored in the root of a shared Heap tree

(min heap or max heap, it doesn't matter) which stores 10*N discrete integers in the 

interval [1 , 10*N], and then adds it to a shared variable int *p, initialized to 0.
