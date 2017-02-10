#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    char *end;
    int n = strtol(argv[1], &end, 10);

    if ( n < 1 )
        return 1;

    for(int i = 0; i < n; ++i){
        if(fork() == 0){
            printf("I am descendent %d\n", i + 1);
            return 0;
        }
        else{
            wait(NULL);
            printf("I am descendent %d\n", i);
        }
    }
}