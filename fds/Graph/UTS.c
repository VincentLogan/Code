#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXVERTEX 1005
#define INFINITY 99999999

char castarray[MAXVERTEX][4];

typedef struct AdjVNode *ptrToAdjVNode;
struct AdjVNode {
    int AdjV;
    int res_capcity;
    ptrToAdjVNode next;
    ptrToAdjVNode reversal;
};

typedef struct Vnode {
    ptrToAdjVNode FirstEdge;
} AdjList[MAXVERTEX];

struct graph {
    int Nv;
    int Ne;
    AdjList G;
} Graph;

struct node {
    int know;
    int fore_AdjV;
    ptrToAdjVNode fore_path;
} Vode[MAXVERTEX];

void Initialize();
int Compare(char *name1, char *name2);
bool BFS(struct node *Vnode);
int Countflow();
void UpdateGraph(int max_flow);

int main() {
    char v1[4], v2[4];
    int Ne;
    int distance;
    int vertex1, vertex2;
    int in_degree = 0, out_degree = 0, flow = 0;
    scanf("%3s %3s %d", v1, v2, &Ne);
    Graph.Ne = Ne;
    Graph.Nv = 2;
    strcpy(castarray[1], v1);
    strcpy(castarray[2], v2);
    for (int i = 1; i <= MAXVERTEX; i++) {
        Graph.G[i].FirstEdge = NULL;
    }
    for (int i = 1; i <= Ne; i++) {
        scanf("%3s %3s %d", v1, v2, &distance);
        vertex1 = 0, vertex2 = 0;
        for (int j = 1; j <= Graph.Nv; j++) {
            if (Compare(v1, castarray[j])) {
                vertex1 = j;
                continue;
            }
            if (Compare(v2, castarray[j])) {
                vertex2 = j;
                continue;
            }
            if (vertex1 != 0 && vertex2 != 0) {
                break;
            }
        }
        if (vertex1 == 1) {
            out_degree += distance;
        }
        if (vertex2 == 2) {
            in_degree += distance;
        }
        if (vertex1 == 0) {
            strcpy(castarray[++Graph.Nv], v1);
            vertex1 = Graph.Nv;
        }
        if (vertex2 == 0) {
            strcpy(castarray[++Graph.Nv], v2);
            vertex2 = Graph.Nv;
        }
        ptrToAdjVNode neVW = (ptrToAdjVNode)malloc(sizeof(struct AdjVNode));
        ptrToAdjVNode neWV = (ptrToAdjVNode)malloc(sizeof(struct AdjVNode));

        neVW->AdjV = vertex2;
        neVW->res_capcity = distance;
        neVW->reversal = neWV;
        neVW->next = Graph.G[vertex1].FirstEdge;
        Graph.G[vertex1].FirstEdge = neVW;

        neWV->AdjV = vertex1;
        neWV->res_capcity = 0;
        neWV->reversal = neVW;
        neWV->next = Graph.G[vertex2].FirstEdge;
        Graph.G[vertex2].FirstEdge = neWV;
    }
    while (BFS(Vode)) {
        int max_flow = Countflow();
        flow += max_flow;
        UpdateGraph(max_flow);
        if (flow == in_degree || flow == out_degree) {
            break;
        }
    }
    printf("%d", flow);
    return 0;
}

void Initialize() {
    for (int i = 1; i <= Graph.Nv; i++) {
        Vode[i].fore_path = NULL;
        Vode[i].fore_AdjV = 0;
        Vode[i].know = 0;
    }
}

int Compare(char *name1, char *name2) {
    int isSame = 1;
    for (int i = 0; i < 3; i++) {
        if (name1[i] != name2[i]) {
            isSame = 0;
            return isSame;
        }
    }
    return isSame;
}

bool BFS(struct node *Vnode) {
    Initialize();
    int queue[MAXVERTEX];
    int front = 0, rear = 0;
    queue[rear++] = 1;
    Vnode[1].know = 1;
    while (front < rear) {
        int pre = queue[front++];
        ptrToAdjVNode currentE = Graph.G[pre].FirstEdge;
        while (currentE) {
            if (currentE->res_capcity > 0 && Vnode[currentE->AdjV].know == 0) {
                Vnode[currentE->AdjV].know = 1;
                Vnode[currentE->AdjV].fore_AdjV = pre;
                Vnode[currentE->AdjV].fore_path = currentE;
                if (currentE->AdjV == 2)
                    return true;
                queue[rear++] = currentE->AdjV;
            }
            currentE = currentE->next;
        }
    }
    return false;
}

int Countflow() {
    int max_flow = INFINITY;
    int pre_AdjV = 2;
    while (pre_AdjV != 1) {
        if (Vode[pre_AdjV].fore_path->res_capcity < max_flow) {
            max_flow = Vode[pre_AdjV].fore_path->res_capcity;
        }
        pre_AdjV = Vode[pre_AdjV].fore_AdjV;
    }
    return max_flow;
}

void UpdateGraph(int max_flow) {
    int pre_AdjV = 2;
    while (pre_AdjV != 1) {
        Vode[pre_AdjV].fore_path->res_capcity -= max_flow;
        Vode[pre_AdjV].fore_path->reversal->res_capcity += max_flow;
        pre_AdjV = Vode[pre_AdjV].fore_AdjV;
    }
}