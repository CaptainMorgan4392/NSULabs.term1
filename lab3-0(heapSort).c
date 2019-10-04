#include <stdio.h>
#include <mm_malloc.h>

void swap(int* a, int* b);
void getElements(int* array, int size, int* flagOfWrongInput);
void buildHeap(int* array, int size);
void heapSort(int* array, int size);

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void getElements(int* array, int size, int* flagOfWrongInput) {
    for (int i = 0; i < size; i++) {
        if (scanf("%d", array + i) == 0) {
            printf("bad input");
            (*flagOfWrongInput)++;
            return;
        }
        buildHeap(array, i + 1);
    }
}

void buildHeap(int* array, int size) {
    int currentIndex = size - 1;
    while (currentIndex != 0) {
        if (array[currentIndex] < array[(currentIndex - 1) / 2]) {
            swap(array + currentIndex, array + (currentIndex - 1) / 2);
            currentIndex = (currentIndex - 1) / 2;
        }
        else break;
    }
}

void heapSort(int* array, int size) {
    while (size > 0) {
        swap(array, array + size - 1);
        printf("%d ", array[size - 1]);
        size--;
        int currentIndex = 0;
        while (1) {
            if (2 * currentIndex + 2 < size && array[2 * currentIndex + 2] < array[2 * currentIndex + 1]) {
                swap(array + currentIndex, array + 2 * currentIndex + 2);
                currentIndex = 2 * currentIndex + 2;
            } else if (2 * currentIndex + 1 < size && array[currentIndex] > array[2 * currentIndex + 1]){
                swap(array + currentIndex, array + 2 * currentIndex + 1);
                currentIndex = 2 * currentIndex + 1;
            } else break;
        }
    }
}

int main() {
    int n;
    if (scanf("%d", &n) == 0) {
        printf("bad input");
    }
    int flagOfWrongInput = 0;
    const int ZERO = 0;
    int* array = (int*)calloc((size_t) n, sizeof(int));
    getElements(array, n, &flagOfWrongInput);
    if (flagOfWrongInput == ZERO) {
        heapSort(array, n);
    }
    free(array);
    return 0;
}
