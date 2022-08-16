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

float length(char *s) {
    int len = 0;
    for (int i = 0; i < sizeof(s); ++i) {
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
    int it = 0;
    for (int i = 0; i < lenOfInput; ++i) {
        char *letter = sliceString(source, it, it);
        printf("%s\n", letter);
        it = it + 1;

    }

    return 0;
}
