#include <stdio.h>
#include <stdlib.h>

int main() {
    int N;
    scanf("%d", &N);
    int origin[N];
    int partial[N];
    int min = 9999999;
    for (int i = 0; i < N; i++) {
        scanf("%d", &origin[i]);
        if (origin[i] < min) {
            min = origin[i];
        }
    }
    for (int i = 0; i < N; i++) {
        scanf("%d", &partial[i]);
    }
    int label1 = 0, label2 = 0;
    int isHeapsort = 1;
    if (partial[0] <= partial[1]) {
        isHeapsort = 0;
    }
    if (!isHeapsort) {
        for (int i = 0; i < N - 2; i++) {
            if (partial[i] > partial[i + 1]) {
                label1 = i + 1;
                break;
            }
        }
        printf("Insertion Sort\n");
        while (partial[label1] < partial[label1 - 1] && label1 >= 1) {
            int temp = partial[label1 - 1];
            partial[label1 - 1] = partial[label1];
            partial[label1] = temp;
            label1--;
        }
    }
    else {
        printf("Heap Sort\n");
        for (int i = N - 1; i > 0; i--) {
            if (partial[i] < partial[0]) {
                int temp = partial[i];
                partial[i] = partial[0];
                partial[0] = temp;
                label2 = i;
                break;
            }
        }
        int max = 1, parent = 0;
        while (max < label2) {
            if (max + 1 < label2 && partial[max + 1] > partial[max]) {
                max++;
            }
            if (partial[parent] < partial[max]) {
                int temp = partial[parent];
                partial[parent] = partial[max];
                partial[max] = temp;
                parent = max;
                max = parent * 2 + 1;
            }
            else {
                break;
            }
        }
    }
    int flag = 1;
    for (int i = 0; i < N; i++) {
        if (flag == 1) {
            printf("%d", partial[i]);
            flag = 0;
        }
        else {
            printf(" %d", partial[i]);
        }
    }
    printf("\n");
    return 0;
}