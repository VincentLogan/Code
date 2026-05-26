#include <stdio.h>
#include <stdlib.h>
#define MAXVERTEX 1005
#define MAXDEGE 200005
#define INFINITY 99999999

struct node {
    int v;
    int w;
    int weight;
} MinPQ[MAXDEGE], temp[MAXDEGE];
int size, Ne, Nv;

int UF[MAXVERTEX];

void Swim(int v);
void Sink(int v);
void Swap(int v, int w);
void LinearBuild();
struct node pop();
int Find(int v);
void Union(int rootv, int rootw);

int main() {
    scanf("%d %d", &Nv, &Ne);
    size = Ne;
    int totalweigh = 0;
    int lastout = -1;
    int isUnique = 1;
    for (int i = 1; i <= Ne; i++) {
        scanf("%d %d %d", &MinPQ[i].v, &MinPQ[i].w, &MinPQ[i].weight);
    }
    LinearBuild();
    for (int i = 1; i <= Nv; i++) {
        UF[i] = -1;
    }
    while (size > 0) {
        int lastout = MinPQ[1].weight;
        int count = 0;
        while (size > 0 && MinPQ[1].weight == lastout) {
            temp[count++] = pop();
        }
        int possible = 0;
        for (int i = 0; i < count; i++) {
            if (Find(temp[i].v) != Find(temp[i].w)) {
                possible++;
            }
        }
        int finial = 0;
        for (int i = 0; i < count; i++) {
            int rootV = Find(temp[i].v);
            int rootW = Find(temp[i].w);
            if (rootV != rootW) {
                Union(rootV, rootW);
                totalweigh += lastout;
                finial++;
            }
        }
        if (possible > finial) {
            isUnique = 0;
        }
    }
    int connected = 0;
    for (int i = 1; i <= Nv; i++) {
        if (UF[i] < 0) {
            connected++;
        }
    }
    if (connected > 1) {
        printf("No MST\n%d\n", connected);
    }
    if (connected == 1) {
        printf("%d\n", totalweigh);
        if (isUnique) {
            printf("Yes\n");
        }
        else {
            printf("No\n");
        }
    }
    return 0;
}

void Swap(int v, int w) {
    struct node temp;
    temp = MinPQ[v];
    MinPQ[v] = MinPQ[w];
    MinPQ[w] = temp;
}

void Swim(int v) {
    int vertex = v;
    int parent = v / 2;
    while (parent >= 1) {
        if (MinPQ[vertex].weight < MinPQ[parent].weight) {
            Swap(vertex, parent);
            vertex = parent;
            parent = vertex / 2;
        }
        else {
            return;
        }
    }
}

void Sink(int v) {
    int vertex = v;
    int small = 2 * vertex;
    while (small <= size) {
        if (small + 1 <= size && MinPQ[small].weight > MinPQ[small + 1].weight) {
            small++;
        }
        if (MinPQ[vertex].weight <= MinPQ[small].weight) {
            return;
        }
        else if (MinPQ[vertex].weight > MinPQ[small].weight) {
            Swap(vertex, small);
            vertex = small;
            small = 2 * vertex;
        }
    }
}

struct node pop() {
    struct node top = MinPQ[1];
    Swap(1, size--);
    Sink(1);
    return top;
}

void LinearBuild() {
    int vertex = size / 2;
    while (vertex) {
        Sink(vertex--);
    }
}

int Find(int v) {
    int root = v;
    while (UF[root] > 0) {
        root = UF[root];
    }
    int pre = v, temp;
    while (pre != root) {
        temp = pre;
        pre = UF[pre];
        UF[temp] = root;
    }
    return root;
}

void Union(int rootv, int rootw) {
    if (UF[rootv] <= UF[rootw]) {
        UF[rootv] += UF[rootw];
        UF[rootw] = rootv;
    }
    else {
        UF[rootw] += UF[rootv];
        UF[rootv] = rootw;
    }
}