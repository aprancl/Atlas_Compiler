#include <stdio.h>

int main() {

    float nums;
    int a;
    int b;
    int c;

    printf("How man fibonacci numbers do you want?\n");
    scanf("%f", &nums);
    printf(" \n");
    a = 0;
    b = 1;
    c = 0;
    while (nums > 0) {
        printf("%.2f\n", (float) (a));
        c = a + b;
        a = b;
        b = c;
        nums = nums - 1;

    }
    return 0;
}
