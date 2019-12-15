#define LENGTH 16
#define NUMBER_OF_CHARS 256
#define bool int
#define false 0
#define true 1

#include <stdio.h>
#include <mm_malloc.h>
#include <memory.h>

FILE* fileIn;

typedef struct _template Template;
typedef struct _ring_buffer RingBuffer;

struct _template {
    unsigned char pattern[LENGTH + 1];
    size_t moves[NUMBER_OF_CHARS];
    size_t size;
};

struct _ring_buffer {
    unsigned char* pattern;
    size_t size;
    size_t curIndexBegin;
    size_t curIndexEnd;
    size_t curIndexInput;
};

void freeDynamicMemory(Template* t, RingBuffer* b) {
    if (t) {
        free(t);
    }
    if (b) {
        free(b -> pattern);
        free(b);
    }
}

bool initialiseTemplate(Template* t) {
    if (fscanf(fileIn, "%16[^\n]s", t -> pattern) == 0) {
        printf("bad input");
        freeDynamicMemory(t, NULL);
        return false;
    }

    if ((unsigned char)fgetc(fileIn) != '\n') {
        printf("bad input");
        freeDynamicMemory(t, NULL);
        return false;
    }

    t -> size = strlen((char*)t -> pattern);
    return true;
}

bool initialiseBuffer(RingBuffer* b, size_t templateSize) {
    b -> pattern = (unsigned char*)calloc(templateSize + 1, sizeof(unsigned char));
    if (fread(b -> pattern, sizeof(unsigned char), templateSize, fileIn) < templateSize) {
        return false;
    }

    b -> size = strlen((char*)b -> pattern);
    b -> curIndexBegin = 0;
    b -> curIndexEnd = b -> size - 1;
    b -> curIndexInput = b -> size;
    return true;
}

void getMoves(Template* t) {
    for (size_t i = 0; i < NUMBER_OF_CHARS; i++) {
        t -> moves[i] = t -> size;
    }

    for (size_t i = 0; i < t -> size - 1; i++) {
        t -> moves[t -> pattern[i]] = t -> size - 1 - i;
    }
}

size_t compareLines(Template* t, RingBuffer* b) {
    for (size_t i = b -> size - 1; i > 0; i--) {
        printf("%zu ", b -> curIndexInput - (b -> size - 1) + i);
        if (t -> pattern[i] != b -> pattern[(b -> size + b -> curIndexEnd + i + 1) % b -> size]) {
            break;
        } else if (i == 1) {
            printf("%zu ", b -> curIndexInput - b -> size + 1);
        }
    }

    size_t curMove = t -> moves[b -> pattern[b -> curIndexEnd]];
    b -> curIndexInput += curMove;

    return curMove;
}

void BoyerMooreSearch(Template* t, RingBuffer* b) {
    while (true) {
        size_t curMove = compareLines(t, b);

        b -> curIndexBegin = (b -> curIndexBegin + curMove) % b -> size;
        size_t writeToBack = b -> size - b -> curIndexEnd - 1;
        size_t firstRead = fread(b -> pattern + (b -> curIndexEnd + 1) % b -> size, sizeof(unsigned char), writeToBack, fileIn);
        size_t secondRead = fread(b -> pattern, sizeof(unsigned char), curMove - writeToBack, fileIn);
        if (feof(fileIn) || firstRead + secondRead != curMove) {
            freeDynamicMemory(t, b);
            return;
        }
        b -> curIndexEnd = (b -> size + b -> curIndexBegin - 1) % b -> size;
    }
}

int main() {
    fileIn = fopen("in.txt", "r");
    Template* template = calloc(1, sizeof(Template));
    if (!initialiseTemplate(template)) {
        return 0;
    }

    getMoves(template);

    RingBuffer* buffer = calloc(1, sizeof(RingBuffer));
    if (!initialiseBuffer(buffer, template -> size)) {
        freeDynamicMemory(template, buffer);
        return 0;
    }

    if (template -> size > buffer -> size) {
        freeDynamicMemory(template, buffer);
        return 0;
    }
    BoyerMooreSearch(template, buffer);
    return 0;
}
