#include <stdio.h>
#include <stdlib.h>

char *makeName() {
    char *data = "Anthony\0";
    return data;
}

int main() {


    makeName();

    return 0;
}
