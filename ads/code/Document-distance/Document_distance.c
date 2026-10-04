#include "stmr.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10007
#define MAX_FILES 100

typedef struct PostingNode {
    int id;
    int count;
    struct PostingNode *next;
} PostingNode;
typedef PostingNode *PtrPostingNode;

typedef struct WordNode {
    char word[21];
    PtrPostingNode postings;
    struct WordNode *next;
} WordNode;
typedef WordNode *PtrWordNode;

PtrWordNode HashTable[HASH_SIZE];
char title[MAX_FILES][7];
int number_of_titles = 0;
double inner_product[MAX_FILES][MAX_FILES];
double distance[MAX_FILES][MAX_FILES];

int HashString(char *word);
PtrWordNode FindWord(char *word);
PtrWordNode CreateWordNode(char *word);
PtrWordNode InsertWord(char *word, int id);
int Readline(int id);
void CalculateDistances(void);
int FindTitle(char *name);
void FreeHashTable(void);

int Readline(int id) {
    char c = getchar();
    if (c == '#') {
        c = getchar();
        if (c == '\n') {
            return 1;
        }
    }
    char word[21];
    while (c != '\n') {
        if (!isalnum(c)) {
            c = getchar();
            continue;
        }
        int i = 0;
        while (isalnum(c)) {
            if (i < 20) {
                word[i++] = tolower(c);
            }
            c = getchar();
        }
        word[i] = '\0';
        int end = stem(word, 0, i - 1);
        word[end + 1] = '\0';
        InsertWord(word, id);
    }
    return 0;
}

int HashString(char *word) {
    unsigned long hashVal = 0;
    char *c = word;
    while (*c != '\0') {
        hashVal = hashVal * 27 + *c++;
    }
    return hashVal % HASH_SIZE;
}

PtrWordNode FindWord(char *word) {
    PtrWordNode current = HashTable[HashString(word)];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

PtrWordNode CreateWordNode(char *word) {
    PtrWordNode newnode = (PtrWordNode)malloc(sizeof(WordNode));
    if (newnode == NULL) {
        return NULL;
    }
    strcpy(newnode->word, word);
    newnode->postings = NULL;
    newnode->next = NULL;
    return newnode;
}

PtrWordNode InsertWord(char *word, int id) {
    PtrWordNode current = FindWord(word);
    if (current != NULL && current->postings->id == id) {
        current->postings->count++;
        return current;
    }

    PtrPostingNode newposting = (PtrPostingNode)malloc(sizeof(PostingNode));
    if (newposting == NULL) {
        return NULL;
    }
    if (current == NULL) {
        current = CreateWordNode(word);
        if (current == NULL) {
            free(newposting);
            return NULL;
        }
        int index = HashString(word);
        current->next = HashTable[index];
        HashTable[index] = current;
    }

    newposting->id = id;
    newposting->count = 1;
    newposting->next = current->postings;
    current->postings = newposting;
    return current;
}

void CalculateDistances(void) {
    for (int i = 0; i < HASH_SIZE; i++) {
        for (PtrWordNode word = HashTable[i]; word != NULL; word = word->next) {
            for (PtrPostingNode p = word->postings; p != NULL; p = p->next) {
                inner_product[p->id][p->id] += (double)p->count * p->count;
                for (PtrPostingNode q = p->next; q != NULL; q = q->next) {
                    double product = (double)p->count * q->count;
                    inner_product[p->id][q->id] += product;
                    inner_product[q->id][p->id] += product;
                }
            }
        }
    }

    double norm[MAX_FILES];
    for (int i = 0; i < number_of_titles; i++) {
        norm[i] = sqrt(inner_product[i][i]);
    }
    for (int i = 0; i < number_of_titles; i++) {
        for (int j = i; j < number_of_titles; j++) {
            double cosine = inner_product[i][j] / (norm[i] * norm[j]);
            if (cosine > 1.0) {
                cosine = 1.0;
            }
            if (cosine < 0.0) {
                cosine = 0.0;
            }
            distance[i][j] = distance[j][i] = acos(cosine);
        }
    }
}

int FindTitle(char *name) {
    for (int i = 0; i < number_of_titles; i++) {
        if (strcmp(title[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

void FreeHashTable(void) {
    for (int i = 0; i < HASH_SIZE; i++) {
        PtrWordNode word = HashTable[i];
        while (word != NULL) {
            PtrWordNode next_word = word->next;
            PtrPostingNode posting = word->postings;
            while (posting != NULL) {
                PtrPostingNode next_posting = posting->next;
                free(posting);
                posting = next_posting;
            }
            free(word);
            word = next_word;
        }
        HashTable[i] = NULL;
    }
}

int main(void) {
    int N, M;
    scanf("%d", &N);
    number_of_titles = N;
    for (int i = 0; i < N; i++) {
        scanf("%6s", title[i]);
        while (Readline(i) == 0) {
            continue;
        }
    }
    CalculateDistances();
    scanf("%d", &M);
    char title1[7], title2[7];
    for (int i = 1; i <= M; i++) {
        scanf("%6s %6s", title1, title2);
        int id1 = FindTitle(title1);
        int id2 = FindTitle(title2);
        printf("Case %d: %.3f\n", i, distance[id1][id2]);
    }
    FreeHashTable();
    return 0;
}
