#define MAX_DEGREE 16
#define SIZE_OF_RING_BUFFER 8192

#include <stdio.h>
#include <mm_malloc.h>
#include <memory.h>
#include <stdbool.h>

typedef struct _template Template;
typedef struct _ring_buffer RingBuffer;

struct _template {
    unsigned char pattern[MAX_DEGREE + 1];
    size_t length;
    size_t hash;
    bool allocated;
};

struct _ring_buffer {
    unsigned char pattern[SIZE_OF_RING_BUFFER + 1];
    size_t length;
    size_t hash;
    bool allocated;
};

unsigned int arrayOfDegrees[MAX_DEGREE];

void calculateDegrees() {
    for (size_t i = 0; i < MAX_DEGREE; i++) {
        arrayOfDegrees[i] = (i == 0 ? 1 : arrayOfDegrees[i - 1] * 3);
    }
}

void getHashFromHash(unsigned char prev, unsigned char next, Template* t, RingBuffer* b) {
    size_t firstTerm = (b -> hash - (prev % 3)) / 3,
           secondTerm = (next % 3) * arrayOfDegrees[t -> length - 1];
    b -> hash = firstTerm + secondTerm;
}

size_t calculateHash(const unsigned char* pattern, size_t length) {
    size_t answer = 0;
    for (size_t i = 0; i < length; i++) {
        size_t firstPart = ((size_t)pattern[i] % 3),
               secondPart = arrayOfDegrees[i];
        answer += firstPart * secondPart;
    }

    return answer;
}

void initialiseTemplate(Template* t) {
    t -> allocated = false;
    if (scanf("%16[^\n]s", t -> pattern) == 0) {
        printf("bad input");
        free(t);
        return;
    }
    t -> allocated = true;
    t -> length = strlen((char*)t -> pattern);
    t -> hash = calculateHash(t -> pattern, t -> length);
    printf("%zu ", t -> hash);

    if (getchar() != '\n') {
        printf("bad input");
        t -> allocated = false;
    }
}

void initialiseBuffer(RingBuffer* b) {
    b -> allocated = false;
    if (scanf("%8192[^\xff]s", b -> pattern) == 0) {
        printf("bad input");
        return;
    }
    b -> allocated = true;
    b -> length = strlen((char*)b -> pattern);
}

void compareLines(const unsigned char* first, const unsigned char* second, size_t currentIndex) {
    for (size_t i = 0; i < strlen((char*)first); i++) {
        printf("%zu ", currentIndex + i);
        if (first[i] != second[i])
            return;
    }
}

void RabinKarpSearch(Template* t, RingBuffer* b) {
    size_t currentIndex = 1;
    bool isBeginOfBuffer = true;
    unsigned char prev = 0;
    unsigned char* blockWithEqualLength = (unsigned char*)calloc(t -> length + 1, sizeof(unsigned char));
    while (true) {
        for (size_t i = 0; i < b -> length - t -> length + 1; i++) {
            if (i == 0) {
                memcpy(blockWithEqualLength, b -> pattern, t -> length);
            } else {
                prev = blockWithEqualLength[0];
                for (size_t j = 1; j < t -> length; j++) {
                    blockWithEqualLength[j - 1] = blockWithEqualLength[j];
                }
                blockWithEqualLength[t -> length - 1] = b -> pattern[t -> length + i - 1];
                currentIndex++;
            }

            if (isBeginOfBuffer) {
                b -> hash = calculateHash(blockWithEqualLength, t -> length);
                isBeginOfBuffer = false;
            } else {
                getHashFromHash(prev, blockWithEqualLength[t -> length - 1], t, b);
            }
            if (t -> hash == b -> hash) {
                compareLines(t -> pattern, blockWithEqualLength, currentIndex);
            }
        }

        if (!feof(stdin) && b -> length == SIZE_OF_RING_BUFFER) {
            isBeginOfBuffer = true;
            initialiseBuffer(b);
            currentIndex += t -> length;
        } else {
            break;
        }
    }

    free(blockWithEqualLength);
}

int main() {
    calculateDegrees();
    Template* template = calloc(1, sizeof(Template));
    initialiseTemplate(template);
    if (!template -> allocated) {
        free(template);
        return 0;
    }
    RingBuffer* buffer = calloc(1, sizeof(RingBuffer));
    initialiseBuffer(buffer);
    if (!buffer -> allocated) {
        free(template);
        free(buffer);
        return 0;
    }
    RabinKarpSearch(template, buffer);
    free(template);
    free(buffer);
    return 0;
}
