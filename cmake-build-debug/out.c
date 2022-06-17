#include <stdio.h>
int main() {

int x;
float pi;

//  for loop examples
for ( int i = 0; i < 10; ++i)
{
printf("for loop as int literal\n");

}
x = 5;
for ( int i = 0; i < x; ++i)
{
printf("for loop as int Variable\n");

}
for ( int i = 0; i < sizeof("Anthony"); ++i)
{
printf("as string literal\n");

}
char name[6] = "Cheems";
for ( int i = 0; i < sizeof(name); ++i)
{
printf("%s\n",name);

}
pi = 3.14;
for ( int i = 0; i < pi; ++i)
{
printf("%.2f\n", (float)(pi));

}

return 0;
}
