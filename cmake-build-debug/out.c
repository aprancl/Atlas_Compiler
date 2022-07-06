#include <stdio.h>
#include <stdlib.h>
int main() {

float nums;

//  Fibonacci
printf("How man fibonacci numbers do you want?\n");
scanf("%f", &nums);
printf(" \n");
int a = 0;
int b = 1;
int c = 0;
while (nums > 0)
{
printf("%.2f\n", (float)(a));
c = a + b;
a = b;
b = c;
nums = nums - 1;

}

return 0;
}
