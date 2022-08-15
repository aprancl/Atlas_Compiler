#include <stdio.h>
#include <stdlib.h>
float giveMeTwo() {
int y = 2;
return y;
}
int main() {


//  variable reassignment testing
int sum = 0;
int x = 1;
x = 4;
sum = sum + x;
x = giveMeTwo();
char *name = "Anthony\0";
//  problem
printf("%s\n",name);

return 0;
}
