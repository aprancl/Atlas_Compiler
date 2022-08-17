#include <stdio.h>
#include <stdlib.h>

float factorial(float n) {
//  returns n! factorial
    if (n == 0) {
        return 1;

    } else {
        float x = n - 1;
        float temp = factorial(x);
        return n * temp;

    }
    return 1;
}

int main() {

    float in;

//  recursive factorial program in atlas
    printf("Please enter a number below\n");
    scanf("%f", &in);
    float answer = factorial(in);
    printf("%.2f\n", (float) (answer));

    return 0;
}
