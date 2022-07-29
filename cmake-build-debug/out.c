#include <stdio.h>
#include <stdlib.h>
int main() {

float x;

//  Simple counting program
printf("Enter a number!\n");
scanf("%f", &x);
int a = 1;
for ( int i = 0; i < x; ++i)
{
printf("%.2f\n", (float)(a));
a = a + 1;

}

return 0;
}
