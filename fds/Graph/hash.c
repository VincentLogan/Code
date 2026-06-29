#include <stdio.h>
#include <stdlib.h>

int HashTable[1005] = {0};
int OriginTable[1005] = {0};
int Adj[1005][1005] = {0};
int Indgree[1005] = {0};
int minPQ[1005] = {0};
int count = 0;

void sink(int index, int size);
void swim(int index);
int pop(int *size);

int main() {
    int N;
    scanf("%d", &N);
    int size = 0;
    for (int i = 0; i < N; i++) {
        scanf("%d", &HashTable[i]);
        if (HashTable[i] < 0) {
            continue;
        }
        if (HashTable[i] % N == i) {
            Indgree[i] = 0;
            minPQ[++size] = HashTable[i];
            swim(size);
        }
        else if (HashTable[i] % N < i) {
            Indgree[i] = i - HashTable[i] % N;
            for (int j = i - Indgree[i]; j < i; j++) {
                Adj[j][i] = 1;
            }
        }
        else {
            Indgree[i] = N - (HashTable[i] % N - i);
            for (int j = 0; j < i; j++) {
                Adj[j][i] = 1;
            }
            for (int j = HashTable[i] % N; j < N; j++) {
                Adj[j][i] = 1;
            }
        }
    }

    while (size != 0) {
        OriginTable[count++] = pop(&size);
        int temp = OriginTable[count - 1];
        int across = -1;
        for (int i = 0; i < N; i++) {
            if (temp == HashTable[i]) {
                across = i;
                break;
            }
        }
        for (int i = 0; i < N; i++) {
            if (Adj[across][i] == 1) {
                Indgree[i]--;
                Adj[across][i] = 0;
                if (Indgree[i] == 0) {
                    minPQ[++size] = HashTable[i];
                    swim(size);
                }
            }
        }
    }

    int flag = 1;
    for (int i = 0; i < count; i++) {
        if (flag) {
            printf("%d", OriginTable[i]);
            flag = 0;
        }
        else {
            printf(" %d", OriginTable[i]);
        }
    }
    printf("\n");
    return 0;
}

void sink(int index, int size) {
    int smallest = index * 2;
    if (smallest > size) {
        return;
    }
    else if (smallest < size && minPQ[smallest + 1] < minPQ[smallest]) {
        smallest++;
    }
    if (minPQ[index] > minPQ[smallest]) {
        int temp = minPQ[index];
        minPQ[index] = minPQ[smallest];
        minPQ[smallest] = temp;
        sink(smallest, size);
    }
    else {
        return;
    }
}

void swim(int index) {
    int parent = index / 2;
    if (parent == 0) {
        return;
    }
    if (minPQ[parent] > minPQ[index]) {
        int temp = minPQ[parent];
        minPQ[parent] = minPQ[index];
        minPQ[index] = temp;
        swim(parent);
    }
    else {
        return;
    }
}

int pop(int *size) {
    int temp = minPQ[1];
    minPQ[1] = minPQ[*size];
    minPQ[(*size)] = 0;
    (*size)--;
    sink(1, *size);
    return temp;
}