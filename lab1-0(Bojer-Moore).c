#define LENGTH 16
#define NUMBER_OF_CHARS 256

#include <stdio.h>
#include <mm_malloc.h>
#include <memory.h>
#include <stdbool.h>

typedef struct _template Template;
typedef struct _ring_buffer RingBuffer;

struct _template {
    unsigned char pattern[LENGTH + 1];
    size_t length;
    size_t moves[NUMBER_OF_CHARS];
    bool initialised;
};

struct _ring_buffer {
    unsigned char* pattern;
    size_t length;
    bool initialised;
};

void calculateMoves(Template* t) {
    for (size_t i = 0; i < NUMBER_OF_CHARS; i++) {
        t -> moves[i] = t -> length;
    }

    for (size_t i = 0; i < t -> length - 1; i++) {
        t -> moves[t -> pattern[i]] = t -> length - i - 1;
    }
}

void initialiseTemplate(Template* t) {
    t -> initialised = false;
    if (scanf("%16[^\n]s", t -> pattern) == 0) {
        printf("bad input");
        return;
    }
    t -> length = strlen((char*)t -> pattern);
    t -> initialised = true;
    calculateMoves(t);

    if (getchar() != '\n') {
        printf("bad input");
        t -> initialised = false;
    }
}

void initialiseBuffer(RingBuffer* b, size_t length) {
    b -> pattern = calloc(length + 1, sizeof(unsigned char));
    b -> initialised = false;
    if (fread(b -> pattern, sizeof(unsigned char), length, stdin) == 0) {
        return;
    }
    b -> initialised = true;
    b -> length = strlen((char*)b -> pattern);
}

size_t compareLinesAndGetMove(Template *t, RingBuffer* b, size_t currentIndex) {
    for (size_t i = t -> length - 1; i > 0; i--) {
        printf("%zu ", currentIndex - (t -> length - 1 - i));
        if (t -> pattern[i] != b -> pattern[i]) {
            break;
        }

        if (i == 1) {
            printf("%zu ", currentIndex - t -> length + 1);
        }
    }

    return t -> moves[b -> pattern[t -> length - 1]];
}

void BoyerMooreSearch(Template* t, RingBuffer* b) {
    size_t currentIndex = t -> length;
    while (true) {
        if (t -> length > b -> length) {
            return;
        }

        size_t currentMove = compareLinesAndGetMove(t, b, currentIndex);
        for (size_t i = currentMove; i < t -> length; i++) {
            b -> pattern[i - currentMove] = b -> pattern[i];
        }

        if (feof(stdin) || fread(b -> pattern + t -> length - currentMove, sizeof(unsigned char), currentMove, stdin) != currentMove) {
            return;
        }

        currentIndex += currentMove;
    }
}

int main() {
    Template* template = calloc(1, sizeof(Template));
    initialiseTemplate(template);
    if (!template -> initialised) {
        free(template);
        return 0;
    }
    RingBuffer* buffer = calloc(1, sizeof(RingBuffer));
    initialiseBuffer(buffer, template -> length);
    if (!buffer -> initialised) {
        free(template);
        free(buffer -> pattern);
        free(buffer);
        return 0;
    }
    BoyerMooreSearch(template, buffer);
    free(template);
    free(buffer -> pattern);
    free(buffer);
    return 0;
}
