#include <stdio.h>
#include <stdlib.h>
int main() {


//  Variable testing
//  Strings
char *name = "Anthony\0";
name = "Bob\0";
char *name2 = "Bob\0";
name2 = "Erick\0";
//  integers
int a = 5;
a = 2;
int b = a;
int x = 100;
b = x;
//  integers
float pi = 3.14;
pi = 3.999;
float q = 5.23;
float y = 1.23;
q = y;
//  expressions
float expr = 1 + 1;
float thing = 8 * 8;
expr = thing + 7;
expr = expr + 100;
float expr2 = 10 * expr;

return 0;
}
