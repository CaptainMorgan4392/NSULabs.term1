#include <stdio.h>
#include <mm_malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <ctype.h>

bool flagOfException = false;

typedef enum {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    NUMBER,
    BEGIN
} Token;

typedef struct _stackInt {
    int* array;
    size_t size;
} stackInt;

typedef struct _stackChar {
    char* array;
    size_t size;
} stackChar;

void createStackInt(stackInt* stack) {
    stack -> array = (int*)calloc(1000, sizeof(int));
}

void createStackChar(stackChar* stack) {
    stack -> array = (char*)calloc(1000, sizeof(char));
}

void pushInt(stackInt* stack, int value) {
    stack -> array[stack -> size++] = value;
}

void pushChar(stackChar* stack, char op) {
    stack -> array[stack -> size++] = op;
}

int peekInt(stackInt* stack) {
    return stack -> array[stack -> size - 1];
}

char peekChar(stackChar* stack) {
    return stack -> array[stack -> size - 1];
}

void popInt(stackInt* stack) {
    if (stack -> size == 0) {
        flagOfException = true;
        return;
    }
    stack -> size--;
}

void popChar(stackChar* stack) {
    if (stack -> size == 0) {
        flagOfException = true;
        return;
    }
    stack -> size--;
}

int calculateBinaryOperation(int first, int second, char op) {
    switch (op) {
        case '+':
            return first + second;
        case '-':
            return first - second;
        case '*':
            return first * second;
        case '/':
            if (second == 0) {
                flagOfException = true;
                return 0;
            }
            return first / second;
        default:
            return -1;
    }
}

void checkExceptions(char* expression, const char* alphabet) {
    Token currentToken = BEGIN;
    int counterOfBrackets = 0;

    for (size_t i = 0; i < strlen(expression); i++) {
        char currentSymbol = expression[i];

        if (NULL == strchr(alphabet, expression[i])) {
            printf("syntax error");
            free(expression);
            exit(1);
        }

        switch (currentSymbol) {
            case '+':
                if (currentToken != CLOSE_BRACKET && currentToken != NUMBER) {
                    printf("syntax error");
                    free(expression);
                    exit(1);
                }
                currentToken = ADD;
                break;
            case '-':
                if (currentToken != CLOSE_BRACKET && currentToken != NUMBER) {
                    printf("syntax error");
                    free(expression);
                    exit(1);
                }
                currentToken = SUBTRACT;
                break;
            case '*':
                if (currentToken != CLOSE_BRACKET && currentToken != NUMBER) {
                    printf("syntax error");
                    free(expression);
                    exit(1);
                }
                currentToken = MULTIPLY;
                break;
            case '/':
                if (currentToken != CLOSE_BRACKET && currentToken != NUMBER) {
                    printf("syntax error");
                    free(expression);
                    exit(1);
                }
                currentToken = DIVIDE;
                break;
            case '(':
                if (currentToken == NUMBER || currentToken == CLOSE_BRACKET) {
                    printf("syntax error");
                    free(expression);
                    exit(1);
                }
                currentToken = OPEN_BRACKET;
                counterOfBrackets++;
                break;
            case ')':
                if (currentToken != NUMBER && currentToken != CLOSE_BRACKET) {
                    printf("syntax error");
                    free(expression);
                    exit(1);
                }
                currentToken = CLOSE_BRACKET;
                counterOfBrackets--;
                if (counterOfBrackets < 0) {
                    printf("syntax error");
                    free(expression);
                    exit(1);
                }
                break;
            default:
                currentToken = NUMBER;
        }
    }

    if (counterOfBrackets != 0 || (currentToken != NUMBER && currentToken != CLOSE_BRACKET)) {
        printf("syntax error");
        free(expression);
        exit(1);
    }
}

int main() {
    char* expression = (char*)calloc(1001, sizeof(char));
    if (scanf("%1000s", expression) == 0) {
        printf("syntax error");
        free(expression);
        exit(1);
    }

    if ((char)getchar() != '\n') {
        printf("syntax error");
        free(expression);
        exit(1);
    }

    const char* alphabet = "0123456789+-*/()";
    checkExceptions(expression, alphabet);

    stackInt* ints = calloc(1, sizeof(stackInt));
    stackChar* chars = calloc(1, sizeof(stackChar));
    createStackInt(ints);
    createStackChar(chars);

    int currentNumber = 0,
        firstOperand = 0,
        secondOperand = 0;
    for (size_t i = 0; i < strlen(expression); i++) {
        char currentSymbol = expression[i];
        switch (currentSymbol) {
            case '+':
                while (chars -> size > 0 && peekChar(chars) != '(') {
                    secondOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    firstOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    pushInt(ints, calculateBinaryOperation(firstOperand, secondOperand, peekChar(chars)));

                    if (flagOfException && peekChar(chars) == '/') {
                        printf("division by zero");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }
                    popChar(chars);
                }
                pushChar(chars, '+');
                break;
            case '-':
                while (chars -> size > 0 && peekChar(chars) != '(') {
                    secondOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    firstOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    pushInt(ints, calculateBinaryOperation(firstOperand, secondOperand, peekChar(chars)));

                    if (flagOfException && peekChar(chars) == '/') {
                        printf("division by zero");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }
                    popChar(chars);
                }
                pushChar(chars, '-');
                break;
            case '*':
                while (chars -> size > 0 && NULL == strchr("(+-", peekChar(chars))) {

                    secondOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    firstOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    pushInt(ints, calculateBinaryOperation(firstOperand, secondOperand, peekChar(chars)));

                    if (flagOfException && peekChar(chars) == '/') {
                        printf("division by zero");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }
                    popChar(chars);
                }
                pushChar(chars, '*');
                break;
            case '/':
                while (chars -> size > 0 && NULL == strchr("(+-", peekChar(chars))) {
                    secondOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    firstOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    pushInt(ints, calculateBinaryOperation(firstOperand, secondOperand, peekChar(chars)));

                    if (flagOfException && peekChar(chars) == '/') {
                        printf("division by zero");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }
                    popChar(chars);
                }
                pushChar(chars, '/');
                break;
            case '(':
                pushChar(chars, '(');
                break;
            case ')':
                while (chars -> size > 0 && peekChar(chars) != '(') {
                    secondOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    firstOperand = peekInt(ints);
                    popInt(ints);
                    if (flagOfException) {
                        printf("syntax error");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }

                    pushInt(ints, calculateBinaryOperation(firstOperand, secondOperand, peekChar(chars)));

                    if (flagOfException && peekChar(chars) == '/') {
                        printf("division by zero");
                        free(expression);
                        free(ints -> array);
                        free(chars -> array);
                        free(ints);
                        free(chars);
                        exit(1);
                    }
                    popChar(chars);
                }
                popChar(chars);
                break;
            default:
                while (isdigit(expression[i])) {
                    currentNumber = 10 * currentNumber + expression[i++] - '0';
                }
                i--;
                pushInt(ints, currentNumber);
                currentNumber = 0;
        }
    }

    while (chars -> size > 0) {
        secondOperand = peekInt(ints);
        popInt(ints);
        if (flagOfException) {
            printf("syntax error");
            free(expression);
            free(ints -> array);
            free(chars -> array);
            free(ints);
            free(chars);
            exit(1);
        }

        firstOperand = peekInt(ints);
        popInt(ints);
        if (flagOfException) {
            printf("syntax error");
            free(expression);
            free(ints -> array);
            free(chars -> array);
            free(ints);
            free(chars);
            exit(1);
        }

        pushInt(ints, calculateBinaryOperation(firstOperand, secondOperand, peekChar(chars)));

        if (flagOfException && peekChar(chars) == '/') {
            printf("division by zero");
            free(expression);
            free(ints -> array);
            free(chars -> array);
            free(ints);
            free(chars);
            exit(1);
        }
        popChar(chars);
    }

    if (ints -> size != 1) {
        printf("syntax error");
    } else {
        printf("%d", ints -> array[0]);
    }

    if (!flagOfException) {
        free(expression);
    }

    free(ints -> array);
    free(chars -> array);
    free(ints);
    free(chars);
    return 0;
}
