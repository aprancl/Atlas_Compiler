#include <stdio.h>
#include <stdlib.h>
int main() {


int x = 1;
int y = 5;
int z = 10;
float p = 1 + x + y;
float q = 1 + p + z - x;
printf("%.2f\n", (float)(q));

return 0;
}
