#include <stdio.h>
#include <stdlib.h>
float doMath(float x,float y) {
float answer = x + y;
return answer;
}
char* sayHi() {
char *msg = "Hi!\0";
return msg;
}
char* sayHello() {
char *messg = "Hello!\0";
return messg;
}
int main() {


//  i want to be able to use <msg> instead of <messg>
doMath(1,2);
sayHi();
printf("%s\n", sayHi());
char *msg = sayHello();
printf("%s\n",msg);

return 0;
}
