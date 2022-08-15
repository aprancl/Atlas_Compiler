#include <stdio.h>
#include <stdlib.h>
char *sliceString(char *str, int start, int end) {
int i;
int size = (end - start) + 2;
char *output = (char *) malloc(size * sizeof(char));
for (i = 0; start <= end; start++, i++) {
output[i] = str[start];

}
output[size] = '\0';
return output;

}
float length(char* s) {
int len = 0;
for ( int i = 0; i < sizeof(s); ++i)
{
len = len + 1;

}
return len;
}
int main() {

char *source = malloc(sizeof(source));
char *target = malloc(sizeof(target));

//  This is a program that will search for a given pattern in a string of text
scanf("%s", source);
scanf("%s", target);
float lenOfInput = length(source);
float i =  - 1;
for ( int i = 0; i < lenOfInput; ++i)
{
i = i + 1;
char *letter = sliceString("source", i+1, i+1);printf("%s\n",letter);

}

return 0;
}
