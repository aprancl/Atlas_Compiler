#include <stdio.h>
#include <stdlib.h>
int main() {


char *name = "Anthony\0";
char *name2 = "Bob\0";
char *name3 = "Bob\0";
name3 = "Anthony";
printf("%s\n",name3);

return 0;
}
