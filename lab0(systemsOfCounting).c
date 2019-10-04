#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>



void getNumber(char str[], int b1, int *flagOfWrongInput, int *lengthOfNumber,
        int *positionOfDot, int *counterOfDots);
void calculateInOtherSystemWhole(long int part, int power);
void calculateInOtherSystemFraction(long double fraction, int power);
void checkExceptions(char currentChar, int currentPosition, int *flagOfWrongInput, int *counterOfDots,
                     int *positionOfDot);
void checkDot(int *lengthOfNumber, int *flagOfWrongInput, int *positionOfDot);
void calculateInDecimalWhole(int *lengthOfNumber, char str[], long int *whole, int *positionOfDot, int b1);
void calculateInDecimalFraction(char str[], int *positionOfDot, long double *fraction, int b1);



void checkExceptions(char currentChar, int currentPosition, int *flagOfWrongInput, int *counterOfDots,
                     int *positionOfDot) {
    if (currentChar == '.') {
        if (*counterOfDots == 1) {
            (*flagOfWrongInput)++;
            return;
        } else {
            (*counterOfDots)++;
            *positionOfDot = currentPosition;
        }
    } else if (!isdigit(currentChar) && !((toupper(currentChar) >= 'A') && (toupper(currentChar) <= 'F'))) {
        (*flagOfWrongInput)++;
        return;
    }
}

void checkDot(int *lengthOfNumber, int *flagOfWrongInput, int *positionOfDot) {
    if ((*positionOfDot) == 0 || (*positionOfDot) == (*lengthOfNumber) - 1) {
        (*flagOfWrongInput)++;
    }
}

void getNumber(char str[], int b1, int *flagOfWrongInput, int *lengthOfNumber,
        int *positionOfDot, int *counterOfDots) {
    for (int i = 0; i < 13; i++) {
        str[i] = 0;
    }
    char currentChar = 0;
    int currentPosition = 0;
    char useless;
    if (scanf("%c", &useless) == 0) {
        (*flagOfWrongInput)++;
        return;
    }
    while (scanf("%c", &currentChar) && currentChar != '\n') {
        checkExceptions(currentChar, currentPosition, flagOfWrongInput, counterOfDots, positionOfDot);
        if (b1 <= 10) {
            if (currentChar - '0' < b1) {
                str[currentPosition] = currentChar;
            } else {
                (*flagOfWrongInput)++;
                return;
            }
        } else {
            if (toupper(currentChar) <= 'A' + b1 - 10) {
                str[currentPosition] = (char) toupper(currentChar);
            } else {
                (*flagOfWrongInput)++;
                return;
            }
        }
        currentPosition++;
    }
    *lengthOfNumber = currentPosition;
    checkDot(lengthOfNumber, flagOfWrongInput, positionOfDot);
}

void calculateInDecimalWhole(int *lengthOfNumber, char str[], long int *whole, int *positionOfDot, int b1) {
    for (int i = 0; ((i < (*positionOfDot) && (*positionOfDot != -1))
    || (i < (*lengthOfNumber) && ((*positionOfDot) == -1))); i++) {
        if (isdigit(str[i])) {
            (*whole) = (*whole) * b1 + str[i] - '0';
        } else {
            (*whole) = (*whole) * b1 + (char)toupper(str[i]) - 'A' + 10;
        }
    }
}

void calculateInDecimalFraction(char str[], int *positionOfDot, long double *fraction, int b1) {
    int currentValue = b1;
    for (int i = (*positionOfDot) + 1; i < 13 && str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            (*fraction) += (long double)(str[i] - '0') / currentValue;
        } else {
            (*fraction) += (long double)((char)toupper(str[i]) - 'A' + 10) / currentValue;
        }
        currentValue *= b1;
        if ((*fraction) > 0.99999999) {
            (*fraction) = 0.99999999;
            break;
        }
    }
}

void calculateInOtherSystemWhole(long int part, int power) {
    long int currentValue = power;
    int degree = 1;
    while (currentValue < part) {
        currentValue *= power;
        degree++;
    }
    if (currentValue != part) {
        currentValue /= power;
        degree--;
        while (degree >= 0) {
            if (power <= 10) {
                printf("%li", part / currentValue);
            } else {
                if (part / currentValue < 10) {
                    printf("%li", part / currentValue);
                } else {
                    printf("%c", (char)('A' +  (part / currentValue) - 10));
                }
            }
            part = part - ((part / currentValue) * currentValue);
            currentValue /= power;
            degree--;
        }
    } else {
        printf("%d", 1);
        for (int i = 0; i < degree; i++) {
            printf("%d", 0);
        }
    }
}

void calculateInOtherSystemFraction(long double fraction, int power) {
    double currentFraction = 1;
    int degree = 1;
    while (degree <= 12 && fraction != 0) {
        currentFraction = currentFraction / (double)power;
        if (fraction / currentFraction < 10) {
            printf("%d", (int)(fraction / currentFraction));
        } else {
            printf("%c", (char)('A' + (int)(fraction / currentFraction) - 10));
        }
        fraction = fraction - (double)((int)(fraction / currentFraction)) * currentFraction;
        degree++;
    }
}



int main() {
    int flagOfWrongInput = 0;
    int lengthOfNumber = 0;
    int counterOfDots = 0, positionOfDot = -1;
    int b1 = 0, b2 = 0;
    if (scanf("%d%d", &b1, &b2) < 2) {
        printf("Bad input\n");
        return 0;
    }
    if (b1 >= 2 && b1 <= 16 && b2 >= 2 && b2 <= 16) {
        char str[13];
        getNumber(str, b1, &flagOfWrongInput, &lengthOfNumber, &positionOfDot, &counterOfDots);
        if (flagOfWrongInput == 0) {
            long int whole = 0;
            if (positionOfDot == -1) {
                calculateInDecimalWhole(&lengthOfNumber, str, &whole, &positionOfDot, b1);
                if (b2 != 10) {
                    calculateInOtherSystemWhole(whole, b2);
                } else {
                    printf("%li", whole);
                }
            } else {
                long double fraction = 0;
                calculateInDecimalWhole(&lengthOfNumber, str, &whole, &positionOfDot, b1);
                calculateInDecimalFraction(str, &positionOfDot, &fraction, b1);
                if (b2 != 10) {
                    calculateInOtherSystemWhole(whole, b2);
                    if (fraction != 0) {
                        printf("%c", '.');
                        calculateInOtherSystemFraction(fraction, b2);
                    }
                } else {
                    printf("%Lf", whole + fraction);
                }
            }
        }
    } else {
        flagOfWrongInput++;
    }
    if (flagOfWrongInput > 0) {
        printf("Bad input\n");
    }
    return 0;
}
