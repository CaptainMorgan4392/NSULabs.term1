#include <stdio.h>
#include <mm_malloc.h>
#include <stdbool.h>
#include <memory.h>
#include <ctype.h>

typedef struct Stack Stack;
typedef enum Token Token;
typedef enum Exceptions Exceptions;
typedef struct Pair Pair;

/* Enumeration of all tokens, which can meet in expression */

enum Token {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NUMBER
};

/* Exceptions */

bool flagOfException = false;

const char* namesOfExceptions[] = {
        "syntax error",
        "division by zero"
};

enum Exceptions {
    PARSING_ERROR = 1,
    UNDEFINED_RESULT
};

/* Struct Pair */

struct Pair {
    int first;
    int second;
};

/* Definition of stack */

struct Stack {
    char** lines;
    int* integers;
    size_t size;
};

void createStackChar(Stack* stack) {
    stack -> lines = (char**)calloc(1000, sizeof(char*));
    for (int i = 0; i < 1000; i++) {
        stack -> lines[i] = (char*)calloc(20, sizeof(char));
    }
    stack -> size = 0;
}

void createStackInt(Stack* stack) {
    stack -> integers = (int*)calloc(1000, sizeof(int));
    stack -> size = 0;
}

bool isEmpty(Stack* stack) {
    return stack -> size == 0;
}

void pushLine(Stack* stack, char* line) {
    stack -> lines[stack -> size++] = line;
}

void pushInt(Stack* stack, int value) {
    stack -> integers[stack -> size++] = value;
}

int peekInt(Stack* stack){
    return stack -> integers[stack -> size - 1];
}

char* peekLine(Stack* stack){
    return stack -> lines[stack -> size - 1];
}

void pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("%s", namesOfExceptions[0]);
        flagOfException = true;
    }
    stack -> size--;
}

/* Method, which is scanning input line and checkin' it for exceptions */

void getLineAndCheckExceptions(char *expression, const char* alphabet) {
    char currentSymbol;
    size_t currentLength = 0;
    int countOfBrackets = 0;

    while ((currentSymbol = (char)getchar()) != '\n') {
        if (NULL == strchr(alphabet, currentSymbol)) {
            printf("%s", namesOfExceptions[0]);
            free(expression);
            exit(PARSING_ERROR);
        }
        if (currentSymbol == '(') {
            countOfBrackets++;
        } else if (currentSymbol == ')') {
            countOfBrackets--;
            if (countOfBrackets < 0) {
                printf("%s", namesOfExceptions[0]);
                free(expression);
                exit(PARSING_ERROR);
            }
        }

        expression[currentLength++] = currentSymbol;
    }

    if (currentLength == 0) {
        printf("%s", namesOfExceptions[0]);
        free(expression);
        exit(PARSING_ERROR);
    } else if (countOfBrackets != 0) {
        printf("%s", namesOfExceptions[0]);
        free(expression);
        exit(PARSING_ERROR);
    }
}

char** splitToTokens(char* expression, size_t length, size_t *quantityOfTokens) {
    char** result = (char**)calloc(1001, sizeof(char*));
    if (!result) {
        printf("No memory?!");
        free(expression);
        return 0;
    }

    for (int i = 0; i < 1001; i++) {
        result[i] = (char*)calloc(20, sizeof(char));
    }

    size_t  currentIndex = 0,
            currentIndexInArrOfTokens = 0;
    bool isDigit = false;
    while (currentIndex < length) {
        char* currentToken = (char*)calloc(20, sizeof(char));
        if (!currentToken) {
            printf("No memory?!");
            free(expression);
            return 0;
        }

        char currentSymbol = expression[currentIndex];
        switch (currentSymbol) {
            case '+':
                strcpy(currentToken, "+");
                break;
            case '-':
                strcpy(currentToken, "-");
                break;
            case '*':
                strcpy(currentToken, "*");
                break;
            case '/':
                strcpy(currentToken, "/");
                break;
            case '(':
                strcpy(currentToken, "(");
                break;
            case ')':
                strcpy(currentToken, ")");
                break;
            default:
                isDigit = true;
                char* number = (char*)calloc(20, sizeof(char));
                while (isdigit(currentSymbol)) {
                    number[strlen(number)] = currentSymbol;
                    currentSymbol = expression[++currentIndex];
                }
                strcpy(currentToken, number);
                free(number);
        }
        if (!isDigit) {
            currentIndex++;
        }
        strcpy(result[currentIndexInArrOfTokens++], currentToken);
        (*quantityOfTokens)++;
        isDigit = false;
        free(currentToken);
    }

    return result;
}

size_t calculateFinalQuantity(char** splitted, const size_t quantityOfTokens) {
    size_t answer = 0;

    for (int i = 0; i < quantityOfTokens; i++) {
        if ((strcmp("(", splitted[i]) != 0) && (strcmp(")", splitted[i]) != 0)) {
            answer++;
        } else {
            continue;
        }
    }

    return answer;
}

void getArrayOfTokens(char** parsed, Token* arrayOfTokens, size_t quantityOfTokens) {
    for (size_t i = 0; i < quantityOfTokens; i++) {
        if (strcmp("+", parsed[i]) == 0) {
            arrayOfTokens[i] = ADD;
        } else if (strcmp("-", parsed[i]) == 0) {
            arrayOfTokens[i] = SUBTRACT;
        } else if (strcmp("*", parsed[i]) == 0) {
            arrayOfTokens[i] = MULTIPLY;
        } else if (strcmp("/", parsed[i]) == 0) {
            arrayOfTokens[i] = DIVIDE;
        } else {
            arrayOfTokens[i] = NUMBER;
        }
    }
}

/* My own atoi */

int stringToInt(char* number) {
    int answer = 0;

    for (size_t i = 0; i < strlen(number); i++) {
        answer = 10 * answer + number[i] - '0';
    }

    return answer;
}

Pair* getOperands(Stack* stack) {
    Pair* newPair = calloc(1, sizeof(Pair));

    newPair -> second = peekInt(stack);
    pop(stack);
    if (flagOfException) return NULL;

    newPair -> first = peekInt(stack);
    pop(stack);
    if (flagOfException) return NULL;

    return newPair;
}

int main() {
    /* Alphabet of expression */

    const char* alphabet = "0123456789+-*/()";

    //1. Allocating and initialising memory of expression
    //2. Checking exceptions

    char* expression = (char*)calloc(1001, sizeof(char));
    if (!expression) {
        printf("No memory?!");
        return 0;
    } else {
        getLineAndCheckExceptions(expression, alphabet);
    }

    //3.Start tokenizing

    size_t  length = strlen(expression),
            quantityOfTokens = 0;
    char** splitted = splitToTokens(expression, length, &quantityOfTokens);

    free(expression);

    //4. Start parsing

    void (*stackWithTypename) (Stack* stack);
    size_t finalQuantityOfTokens = calculateFinalQuantity(splitted, quantityOfTokens);

    char** parsed = (char**)calloc(finalQuantityOfTokens, sizeof(char*));

    stackWithTypename = createStackChar;
    Stack* bufferOperations = calloc(1, sizeof(Stack));
    stackWithTypename(bufferOperations);

    size_t currentIndex = 0;

    for (int i = 0; i < quantityOfTokens; i++) {
        char* current = splitted[i];

        if (strcmp("(", current) == 0) {
            pushLine(bufferOperations, current);
        } else if (strcmp(")", current) == 0) {
            while (!isEmpty(bufferOperations) && strcmp(peekLine(bufferOperations), "(") != 0) {
                parsed[currentIndex++] = peekLine(bufferOperations);
                pop(bufferOperations);
            }
            pop(bufferOperations);
        } else if (strcmp("+", current) == 0) {
            while (!isEmpty(bufferOperations) && strcmp(peekLine(bufferOperations), "(") != 0) {
                parsed[currentIndex++] = peekLine(bufferOperations);
                pop(bufferOperations);
            }
            pushLine(bufferOperations, current);
        } else if (strcmp("-", current) == 0) {
            while (!isEmpty(bufferOperations) && strcmp(peekLine(bufferOperations), "(") != 0) {
                parsed[currentIndex++] = peekLine(bufferOperations);
                pop(bufferOperations);
            }
            pushLine(bufferOperations, current);
        } else if (strcmp("*", current) == 0) {
            while (!isEmpty(bufferOperations) && strcmp(peekLine(bufferOperations), "(") != 0 &&
                    strcmp(peekLine(bufferOperations), "+") != 0 && strcmp(peekLine(bufferOperations), "-") != 0) {
                parsed[currentIndex++] = peekLine(bufferOperations);
                pop(bufferOperations);
            }
            pushLine(bufferOperations, current);
        } else if (strcmp("/", current) == 0) {
            while (!isEmpty(bufferOperations) && strcmp(peekLine(bufferOperations), "(") != 0 &&
                   strcmp(peekLine(bufferOperations), "+") != 0 && strcmp(peekLine(bufferOperations), "-") != 0) {
                parsed[currentIndex++] = peekLine(bufferOperations);
                pop(bufferOperations);
            }
            pushLine(bufferOperations, current);
        } else {
            parsed[currentIndex++] = current;
        }
    }

    while (!isEmpty(bufferOperations)) {
        parsed[currentIndex++] = peekLine(bufferOperations);
        pop(bufferOperations);
    }

    for (int i = 0; i < finalQuantityOfTokens; i++) {
        printf("%s ", parsed[i]);
    }
    printf("\n");

    for (int i = 0; i < quantityOfTokens; i++) {
        free(splitted[i]);
    }
    free(splitted);

    //5. Calculating of RPN which we have after parsing
    stackWithTypename = createStackInt;
    Stack* calculatingStack = calloc(1, sizeof(Stack));
    stackWithTypename(calculatingStack);
    Token* arrayOfTokens = calloc(finalQuantityOfTokens, sizeof(Token));

    getArrayOfTokens(parsed, arrayOfTokens, finalQuantityOfTokens);

    for (size_t i = 0; i < finalQuantityOfTokens; i++) {
        Token currentToken = arrayOfTokens[i];
        Pair* operands;
        switch (currentToken) {
            case NUMBER:
                pushInt(calculatingStack, stringToInt(parsed[i]));
                break;
            case ADD:
                operands = getOperands(calculatingStack);
                if (flagOfException) {
                    printf("%s", namesOfExceptions[0]);

                    free(arrayOfTokens);
                    for (size_t j = 0; j < finalQuantityOfTokens; j++) {
                        free(parsed[j]);
                    }
                    free(parsed);

                    exit(PARSING_ERROR);
                }
                pushInt(calculatingStack, operands -> first + operands -> second);
                break;
            case SUBTRACT:
                operands = getOperands(calculatingStack);
                if (flagOfException) {
                    printf("%s", namesOfExceptions[0]);

                    free(arrayOfTokens);
                    for (size_t j = 0; j < finalQuantityOfTokens; j++) {
                        free(parsed[j]);
                    }
                    free(parsed);

                    exit(PARSING_ERROR);
                }
                pushInt(calculatingStack, operands -> first - operands -> second);
                break;
            case MULTIPLY:
                operands = getOperands(calculatingStack);
                if (flagOfException) {
                    printf("%s", namesOfExceptions[0]);

                    free(arrayOfTokens);
                    for (size_t j = 0; j < finalQuantityOfTokens; j++) {
                        free(parsed[j]);
                    }
                    free(parsed);

                    exit(PARSING_ERROR);
                }
                pushInt(calculatingStack, operands -> first * operands -> second);
                break;
            case DIVIDE:
                operands = getOperands(calculatingStack);
                if (flagOfException) {
                    printf("%s", namesOfExceptions[0]);

                    free(arrayOfTokens);
                    for (size_t j = 0; j < finalQuantityOfTokens; j++) {
                        free(parsed[j]);
                    }
                    free(parsed);

                    exit(PARSING_ERROR);
                } else if (operands -> second == 0) {
                    printf("%s", namesOfExceptions[1]);

                    free(arrayOfTokens);
                    for (size_t j = 0; j < finalQuantityOfTokens; j++) {
                        free(parsed[j]);
                    }
                    free(parsed);

                    exit(UNDEFINED_RESULT);
                }
                pushInt(calculatingStack, operands -> first / operands -> second);
                break;
        }
    }

    if (calculatingStack -> size != 1) {
        printf("%s", namesOfExceptions[0]);
    } else {
        printf("%d", peekInt(calculatingStack));
    }

    /*for (size_t j = 0; j < finalQuantityOfTokens; j++) {
        free(parsed[j]);
    }
    free(parsed);
    free(arrayOfTokens);*/
    return 0;
}
