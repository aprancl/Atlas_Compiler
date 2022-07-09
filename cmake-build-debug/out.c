#include <stdio.h>
#include <stdlib.h>
int main() {


char *name = "Anthony\0";
char *name2 = "Bob\0";
name2 = "Anthony\0";
printf("%s\n",name2);

return 0;
}
