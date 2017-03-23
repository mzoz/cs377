#include <stdlib.h>
#include <stdio.h>

int main() {
    int* a = malloc(sizeof(int));
    *a = 10;
    printf("%d\n", *a);
    free(a);
    printf("%d\n", *a);
}
