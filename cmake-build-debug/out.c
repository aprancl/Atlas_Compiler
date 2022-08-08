#include <stdio.h>
#include <stdlib.h>

float doMath(float x, float y) {
    float answer = x + y;
    return answer;
}

char *sayHi() {
    char *msg = "Hello!\0";
    return msg;
}

int main() {


    doMath(1, 2);
    sayHi();
    printf("%s\n", sayHi());

    return 0;
}
