#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INFINITY 99999999
#define MAXVERTEX 1005

// The node of vertex
struct node {
    int know; /*If shortest path is confirmed, know = 1,and know =  0 otherwise*/
    int dist; /*Current shortest distance from source*/
} Vnode[MAXVERTEX];

// The node of edge + adjacent vertex
typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode {
    int AdjV;           /*The index of the adjacent vertex*/
    int weigh;          /*The weight of this edge*/
    PtrToAdjVNode next; /*Pointer to the next adjacent node*/
};

// Adjacency list: a array with element "list of AdjVNode",present all the node connected with i
typedef struct Vnode {
    PtrToAdjVNode FirstEdge; /*Pointer to the first edge of the vertex*/
} AdjList[MAXVERTEX];

// The struct of Graph contained the number of Vertices,Edges and the AdjList
struct {
    int Nv;    /*Total number of vertices*/
    int Ne;    /*Total number of edges*/
    AdjList G; /*Adjacency list to store the graph*/
} Graph;

// The Min Pirority Queue node
struct MinPQNode {
    int v;    /*The index of the vertex*/
    int dist; /*The current shortest distance used as the sorting key*/
};

// The Min Pirority Queue for search the shortest path
struct {
    struct MinPQNode shortestPath[MAXVERTEX]; /*The array to maintain the min priority queue*/
    int size;                                 /*Current number of elements in the MinPQ*/
    int pos[MAXVERTEX]; /*Map the vertex index to its position in the heap array*/
} MinPQ;

void Initialize_Vnode(struct node *Vnode, int S);
bool Verification_Dense(struct node *Vnode, int *sequence);
void Initialize_MinPQ(int S);
void Swap(int index1, int index2);
void Swim(int index);
void Sink(int index);
void Pop();
bool Verification_Sparse(struct node *Vnode, int *sequence);

/*
    Main function of the program

    Paramenter:
    argv[]: the file test for the program

    The method to run this program(in terminal):
    1.Liunx:    cd the directory containing main.c
                gcc -o main main.c
                ./main test/test1.txt

    2.Windows:  cd the directory containing main.c
                gcc -o main.exe main.c
                main.exe test\test1.txt

    3.MacOS:    cd the directory containing main.c
                clang(/gcc) -o main main.c
                ./main test/test1.txt

    The output:
    1. YES/NO
*/
int main(int arc, char *argv[]) {
    int Nv, Ne;
    freopen(argv[1], "r", stdin); /*Open the file provided as inpput*/
    scanf("%d %d", &Nv, &Ne);     /*Read the size of vertices and edges*/
    Graph.Nv = Nv;
    Graph.Ne = Ne;
    int v, w, distance;

    // Initialize the adjacency list for all vertices
    for (int i = 1; i <= Graph.Nv; i++) {
        Graph.G[i].FirstEdge = NULL;
    }

    // Build the undirected graph from the input
    for (int i = 0; i < Graph.Ne; i++) {
        scanf("%d %d %d", &v, &w, &distance); /*read the path and the distance of the path*/

        // build a ne node to represent the edge of v -> w
        PtrToAdjVNode neVW = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
        neVW->AdjV = w;
        neVW->weigh = distance;
        neVW->next = Graph.G[v].FirstEdge;
        Graph.G[v].FirstEdge = neVW;

        // build a ne node to represent the edge of w -> v
        PtrToAdjVNode neWV = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
        neWV->AdjV = v;
        neWV->weigh = distance;
        neWV->next = Graph.G[w].FirstEdge;
        Graph.G[w].FirstEdge = neWV;
    }

    int K, sequence[Nv];
    scanf("%d", &K); /*Read the number of sequences*/

    // Verify each sequence query
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < Graph.Nv; j++) {
            scanf("%d", &sequence[j]);
        } /*Read and initialize the sequence*/
        Initialize_Vnode(Vnode, sequence[0]); /*Initialize vertices status for the new sequence*/

        bool vaild; /*The singal representing whether a sequence is vaild*/
        if (Graph.Ne > 10 * Graph.Nv) {
            /*Use array-based verification for dense graph*/
            vaild = Verification_Dense(Vnode, sequence);
        }
        else {
            /*Use MinPQ verification for sparse graph to reduce time complexity*/
            Initialize_MinPQ(sequence[0]); /*Initialized the MinPQ*/
            vaild = Verification_Sparse(Vnode, sequence);
        }

        if (vaild) {
            printf("YES\n"); /*Valid Dijkstra sequence*/
        }
        else {
            printf("NO\n"); /*Invalid sequence*/
        }
    }
    return 0;
}

/*
    Initialize_Vnode:

    Parameter explanation:
        Vnode: the array of vertices
        S: the source vertex of the current sequence
    Returns:
        None
*/
void Initialize_Vnode(struct node *Vnode, int S) {
    for (int i = 1; i <= Graph.Nv; i++) {
        Vnode[i].dist = INFINITY; /*Reset distance to infinity*/
        Vnode[i].know = 0;        /*Reset status to unknown*/
    }
    Vnode[S].dist = 0; /*Distance of the starting point is 0*/
}

/*
    Verification_Dense:

    Parameter explanation:
        Vnode: the array of vertices
        sequence: the target sequence to be verified
    Returns:
        true if valid, false otherwise

    Time complexity of this function is O(|V|^2 + |E|).
    It is more efficient for dense graphs due to continuous memory access.
*/
bool Verification_Dense(struct node *Vnode, int *sequence) {
    bool vaildSequence = true; /*Initialize the vaild-singal*/
    for (int i = 0; i < Graph.Nv; i++) {
        int currentV = sequence[i];                // verify the sequence[i]

        for (int j = 1; j <= Graph.Nv; j++) {
            if (Vnode[j].know == 0 && Vnode[j].dist < Vnode[currentV].dist) {
                /*If we can found a closer unknown vertex, the sequence is invalid*/
                vaildSequence = false; /*update the singal*/
                return vaildSequence;  /*Return to reduce time consumption*/
            }
        }

        Vnode[currentV].know = 1;                             /*Confirm the current vertex*/
        PtrToAdjVNode currentE = Graph.G[currentV].FirstEdge; /*update the neighbor of the vertex*/

        while (currentE) {                         /*Loop until all the ccontent is updated*/
            if (Vnode[currentE->AdjV].know == 0) { /*Only update the unknown vertex*/
                if (Vnode[currentE->AdjV].dist > Vnode[currentV].dist + currentE->weigh) {
                    /*Found a shorter parh and Update*/
                    Vnode[currentE->AdjV].dist = Vnode[currentV].dist + currentE->weigh;
                }
            }
            currentE = currentE->next; /*move the pointer to update next neigbor*/
        }
    }
    return vaildSequence;
}

/*
    Initialize_MinPQ:

    Parameter explanation:
        S: the source vertex of the current sequence
    Returns:
        None
    Since the distance of starting point is shortest and others is infinity, Building the minPQ with
   the orignal sequence is vaild
*/
void Initialize_MinPQ(int S) {
    MinPQ.size = 1; /*Start filling the MinPQ from index 1*/
    for (int i = 1; i <= Graph.Nv; i++) {
        if (i != S) { /*if i isn't the starting point ,insert into the tail of MinPQ*/
            MinPQ.size++;
            MinPQ.shortestPath[MinPQ.size].dist = INFINITY; /*initialization*/
            MinPQ.shortestPath[MinPQ.size].v = i;           /*Recod the vertex of this distance*/
            MinPQ.pos[i] = MinPQ.size;                      /*Record the position of vertex i*/
        }
        else {
            MinPQ.shortestPath[1].dist = 0; /*The distance of starting point is 0 */
            MinPQ.shortestPath[1].v = S;    /*Recod the vertex of this distance*/
            MinPQ.pos[S] = 1;               /*Record the position of vertex S*/
        }
    }
}

/*
    Swap:

    Parameter explanation:
        index1: the heap index of the first node
        index2: the heap index of the second node
    Returns:
        None
*/
void Swap(int index1, int index2) {
    // Swap the position of two node in minPQ
    struct MinPQNode cache = MinPQ.shortestPath[index1];
    MinPQ.shortestPath[index1] = MinPQ.shortestPath[index2];
    MinPQ.shortestPath[index2] = cache;

    // Update the position mapping array after swapping
    MinPQ.pos[MinPQ.shortestPath[index1].v] = index1;
    MinPQ.pos[MinPQ.shortestPath[index2].v] = index2;
}

/*
    Swim:

    Parameter explanation:
        index: the current heap index of the node that needs to move up
    Returns:
        None
*/
void Swim(int index) {
    if (index == 1) {
        return; /*Stop when reach the top of the heap*/
    }
    if (MinPQ.shortestPath[index].dist < MinPQ.shortestPath[index / 2].dist) {
        /*Smaller than parent, swap and continue to swim*/
        Swap(index, index / 2);
        Swim(index / 2);
    }
}

/*
    Sink:

    Parameter explanation:
        index: the current heap index of the node that needs to move down
    Returns:
        None
*/
void Sink(int index) {
    int left = index * 2;      /*Initialize the left child*/
    int right = index * 2 + 1; /*initialze the right child*/
    int small = index;         /*Initialize the smallest one of index left and right*/

    if (left <= MinPQ.size && MinPQ.shortestPath[small].dist > MinPQ.shortestPath[left].dist) {
        /*Update small index if the left child is in minPQ and is smaller*/
        small = left;
    }
    if (right <= MinPQ.size && MinPQ.shortestPath[small].dist > MinPQ.shortestPath[right].dist) {
        /*Update small index if the right child is in minPQ and is smaller*/
        small = right;
    }
    if (small != index) {
        /*Swap with the smallest child and continue to sink*/
        Swap(index, small);
        Sink(small);
    }
}

/*
    Pop:

    Parameter explanation:
        None
    Returns:
        None
*/
void Pop() {
    Swap(1, MinPQ.size); /*Change the top of minPQ with the end of minPQ*/
    MinPQ.size--;        /*Remove it by reducing the size*/
    Sink(1);             /*Sink the new top node to maintain heap property*/
}

/*
    Verification_Sparse:

    Parameter explanation:
        Vnode: the array of vertices
        sequence: the target sequence to be verified
    Returns:
        true if valid, false otherwise

    Time complexity of this function is O(|E|log|V|).
    Use min-heap and lazy deletion to process sparse graphs efficiently.
*/
bool Verification_Sparse(struct node *Vnode, int *sequence) {
    bool vaildSequence = true; /*Initialize the vaild-singal*/
    for (int i = 0; i < Graph.Nv; i++) {
        int currentV = sequence[i];                // verify the sequence[i]

        while (Vnode[MinPQ.shortestPath[1].v].know == 1) {
            /*Lazy deletion: pop out confirmed vertices sitting at the heap top*/
            Pop();
        }

        if (Vnode[currentV].dist != MinPQ.shortestPath[1].dist) {
            /*The current vertex does not meet the minimum distance requirement*/
            vaildSequence = false; /*update the singal*/
            return false;          /*Return to reduce time consumption*/
        }

        Vnode[currentV].know = 1;                             /*Confirm the current vertex*/
        PtrToAdjVNode currentE = Graph.G[currentV].FirstEdge; /*update the neighbor of the vertex*/

        while (currentE) {                         /*Loop until all the ccontent is updated*/
            if (Vnode[currentE->AdjV].know == 0) { /*Only update the unknown vertex*/
                if (Vnode[currentE->AdjV].dist > Vnode[currentV].dist + currentE->weigh) {
                    /*Relaxation: update distance*/
                    Vnode[currentE->AdjV].dist = Vnode[currentV].dist + currentE->weigh;

                    /* swim the node up in the heap*/
                    int posRefresh = MinPQ.pos[currentE->AdjV];
                    MinPQ.shortestPath[posRefresh].dist = Vnode[currentE->AdjV].dist;
                    Swim(posRefresh);
                }
            }
            currentE = currentE->next; /*move the pointer to update next neigbor*/
        }
    }
    return vaildSequence;
}