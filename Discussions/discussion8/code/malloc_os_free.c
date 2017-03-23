#include <stdlib.h>
#include <stdio.h>

int main() {
    int num = 1;
    int size=1024*1024*1024*sizeof(char); //~1GB

    int* p = (int*) calloc(num, size);

    // prevent compiler from optimizing out the call
    while(1) {
        if(!p) break;
    }
}
