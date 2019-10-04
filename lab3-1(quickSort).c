#include <stdio.h>
#include <mm_malloc.h>

void swap(int* a, int* b);
int partition(int* array, int l, int r);
void qSort(int* array, int nSt, int nEnd);

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* array, int l, int r) {
    int x = array[(l + r) / 2];
    while (l <= r) {
        while (array[l] < x) l++;
        while (array[r] > x) r--;
        if (l >= r) break;
        swap(array + l++, array + r--);
    }
    return r;
}

void qSort(int* array, int nSt, int nEnd) {
    if (nSt >= nEnd) {
        return;
    }
    int l = nSt, r = nEnd;
    int x = partition(array, l, r);
    qSort(array, l, x);
    qSort(array, x + 1, r);
}

int main() {
    int n;
    if (scanf("%d", &n) == 0) {
        printf("bad input");
        return 0;
    }
    int* array = (int*)calloc((size_t) n, sizeof(int));
    for (int i = 0; i < n; i++) {
        if (scanf("%d", array + i) == 0) {
            printf("bad input");
            return 0;
        }
    }
    qSort(array, 0, n - 1);
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    free(array);
    return 0;
}
