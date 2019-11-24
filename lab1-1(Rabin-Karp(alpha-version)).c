#define MAX_DEGREE 16
#define SIZE_OF_BUFFER 4096

#include <stdio.h>
#include <mm_malloc.h>
#include <memory.h>
#include <string.h>
#include <stdbool.h>

unsigned int arrayOfDegrees[16];

typedef struct {
    unsigned char pattern[MAX_DEGREE + 1];
    size_t size;
    unsigned int hash;
} Template;

typedef struct {
    unsigned char pattern[SIZE_OF_BUFFER + 1];
    size_t size;
    unsigned int hash;
} Buffer;

typedef struct {
    Template* t;
    Buffer* b;
} Context;

void (*currentAction)(Context* ctx);

void freeDynamicMemory(Context* ctx) {
    free(ctx -> t);
    free(ctx -> b);
    free(ctx);
}

void calculateDegrees() {
    for (unsigned int i = 0; i < MAX_DEGREE; i++) {
        arrayOfDegrees[i] = (i == 0 ? 1 : arrayOfDegrees[i - 1] * 3);
    }
}

unsigned int calculateHash(const unsigned char* pattern) {
    unsigned int answer = 0;

    for (size_t i = 0; i < MAX_DEGREE; i++) {
        if (pattern[i] == '\0') {
            return answer;
        }
        answer += (pattern[i] % 3) * arrayOfDegrees[i];
    }

    return answer;
}

void initialiseTemplate(Context* ctx) {
    if (scanf("%16[^\n]s", ctx -> t -> pattern) == 0) {
        printf("bad input");
        freeDynamicMemory(ctx);
        return;
    } else {
        unsigned char useless = (unsigned char)getchar();
        if (useless != '\n') {
            printf("bad input");
            freeDynamicMemory(ctx);
            return;
        }
    }

    ctx -> t -> size = strlen((char*)ctx -> t -> pattern);
    ctx -> t -> hash = calculateHash(ctx -> t -> pattern);
    printf("%d ", ctx -> t -> hash);
}

void initialiseBuffer(Context* ctx) {
    if (scanf("%4096[^\xff]s", ctx -> b -> pattern) == 0) {
        freeDynamicMemory(ctx);
        return;
    }

    ctx -> b -> size = strlen((char*)ctx -> b -> pattern);
}

void initialiseContext(Context* ctx) {
    ctx -> t = calloc(1, sizeof(Template));
    currentAction = initialiseTemplate;
    currentAction(ctx);
    ctx -> b = calloc(1, sizeof(Buffer));
    currentAction = initialiseBuffer;
    currentAction(ctx);
}

bool areHashesEqual(Context* ctx) {
    return ctx -> t -> hash == ctx -> b -> hash;
}

void compareLines(Template* t, const unsigned char* toCompare, size_t currentIndex) {
    for (size_t i = 0; i < t -> size; i++) {
        printf("%zu ", currentIndex + i);
        if (t -> pattern[i] != toCompare[i]) {
            return;
        }
    }
}

void startAlgorithm(Context* ctx) {
    static size_t currentIndex = 1;
    size_t currentPositionInBuffer = 0;
    unsigned char* blockWithEqualLength = (unsigned char*)calloc(MAX_DEGREE + 1, sizeof(unsigned char));

    for (size_t i = 0; i < ctx -> b -> size - ctx -> t -> size + 1; i++) {
        if (i == 0) {
            memcpy(blockWithEqualLength, ctx -> b -> pattern, ctx -> t -> size);
            currentPositionInBuffer = strlen((char*)blockWithEqualLength);
        } else {
            memcpy(blockWithEqualLength, blockWithEqualLength + 1, ctx -> t -> size - 1);
            blockWithEqualLength[ctx -> t -> size - 1] = ctx -> b -> pattern[currentPositionInBuffer - 1];
        }

        ctx -> b -> hash = calculateHash(blockWithEqualLength);
        if (areHashesEqual(ctx)) {
            compareLines(ctx -> t, blockWithEqualLength, currentIndex);
        }
        currentIndex++;
        currentPositionInBuffer++;
    }

    if (!feof(stdin)) {
        startAlgorithm(ctx);
    }
}

int main() {
    calculateDegrees();
    Context* context = calloc(1, sizeof(Context));
    currentAction = initialiseContext;
    currentAction(context);
    currentAction = startAlgorithm;
    currentAction(context);
    return 0;
}
