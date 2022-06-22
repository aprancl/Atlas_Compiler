#include <stdio.h>

int main() {

    float x;
    int a;

//  Simple counting program
    printf("Enter a number!\n");
    scanf("%f", &x);
    a = 1;
    for (int i = 0; i < x; ++i) {
        printf("%.2f\n", (float) (a));
        a = a + 1;

    }

    return 0;
}
