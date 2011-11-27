/*
 * =====================================================================================
 *
 *       Filename:  solovay-strassen.c
 *
 *    Description:  The Solovay-Strassen primality test for an odd integer n
 *
 *        Version:  1.0
 *        Created:  11/18/2011 06:01:06 PM
 *       Revision:  0
 *       Compiler:  gcc
 *
 *         Author:  Lim Jeong Sheng
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <math.h>

#define POSITIVE 1;
#define NEGATIVE -1;

typedef struct {
    long numerator;
    long denominator;
    int sign; // true indicate positive, false indicate negative
    int exponent; // only used for property 2
} fraction;

fraction prop1(fraction);
fraction prop2(fraction);
fraction prop3(fraction);
fraction prop4(fraction);
int compute(fraction);
int fractionToInt(fraction);
int isEven(long);
void printFraction(fraction);

int main() {
    fraction input;
    int result;
    input.sign = POSITIVE;
    printf("Please enter a fraction input. For example:\n");
    printf("%34s\n", "Numerator");
    printf("%35s\n", "-----------");
    printf("%35s\n\n", "Denominator");
    printf("Numerator: ");
    scanf("%ld", &input.numerator);
    printf("Denominator: ");
    scanf("%ld", &input.denominator);
    result = compute(input);
    printf("\n\nResult = %d", result);
    return 0;
}

int compute(fraction input) {
    int result;
    do {
        // Once the numerator become 1, then the whole thing will be 1
        if(input.numerator == 1) {
            input.denominator = 1;
            break;
        }
        // If numerator is even excluding 2, since if the numerator
        // is 2, then it should go to prop 2
        if(isEven(input.numerator) > 0 && input.numerator != 2) {
            input = prop3(input);
            // after prop 3 must be prop 2
            printf("\nProp 2\n");
            printFraction(input);
        }
        if(input.numerator == 2 && !isEven(input.denominator)) {
            input = prop2(input);
            // Since this is last step, so we manually set the 
            // value and break the iteration
            input.numerator = 1;
            input.denominator = 1;
            printf("\nProp 2\n");
            printFraction(input);
            break;
        }
        if(input.numerator < input.denominator) {
            input = prop4(input);
            printf("\nProp 4\n");
            printFraction(input);
        }
        if(input.numerator > input.denominator) {
            input = prop1(input);
            printf("\nProp 1\n");
            printFraction(input);
        }
    } while(input.numerator > 0 && input.denominator > 0);
    result = fractionToInt(input);
    return result;
}

fraction prop1(fraction input) {
    input.numerator = input.numerator % input.denominator;
    return input;
}

fraction prop2(fraction input) {
    long n = input.denominator % 8;
    if(n != 1 || n != 3) {
        n += 8;
    }
    int val =  (n == 1) ? 1 : -1;
    val = pow(val, input.exponent);
    if(val == 1) { input.sign = POSITIVE; }
    else { input.sign = NEGATIVE; }
    return input;
}

fraction prop3(fraction input) {
    fraction temp = input;
    temp.numerator = 2;
    int exponent = 0;
    while(isEven(input.numerator) > 0) {
        input.numerator /= 2;
        exponent++;
    }
    temp.exponent = exponent;
    temp = prop2(temp); // Just want to know the SIGN
    if(temp.sign == -1) { // if sign is negative
        // i.e. -1 x -1 = 1
        input.sign *= temp.sign;
    }
    printf("\nProp 3:\n");
    printFraction(temp);
    printf("^%d x ", temp.exponent);
    printFraction(input);
    return input;
}

fraction prop4(fraction input) {
    // swapping
    long denominator = input.denominator;
    input.denominator = input.numerator;
    input.numerator = denominator;

    int mod_denominator = input.denominator % 4;
    int mod_numerator = input.numerator % 4;

    if(mod_numerator == 3 && mod_denominator == 3) {
        input.sign *= NEGATIVE;
    }
    return input;
}

int fractionToInt(fraction input) {
    int result = (int)(input.numerator / input.denominator);
    return (input.sign * result);
}

int isEven(long num) {
    return (num % 2 == 0) ? 1 : 0;
}

void printFraction(fraction frac) {
    if(frac.sign == -1) printf("-");
    printf("(%ld/%ld)", frac.numerator, frac.denominator);
}
