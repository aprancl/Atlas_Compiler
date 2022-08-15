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

int main() {


    char *name = "Anthony\0";
    char *newName = sliceString("Anthony", 2, 4);
    printf("%s\n", newName);

    return 0;
}
