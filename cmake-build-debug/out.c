#include <stdio.h>
#include <stdlib.h>

float giveMeOne() {
    int x = 1;
    return x;
}

float giveMeTwo() {
    int y = 2;
    return y;
}

int main() {


//  working with functions and numbers
    float a = giveMeOne();
    float b = giveMeTwo();
    printf("%.2f\n", (float) (a + b));

    return 0;
}
