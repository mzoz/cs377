#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int a;
    int b;
} myarg_t;

void* worker(void* arg) {
    myarg_t *m = (myarg_t *) arg;
    unsigned int seconds = 1;
    sleep(seconds);
    printf("The sum of input is %d\n", m->a + m->b);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;

    myarg_t args = {
        .a = 3,
        .b = 4,
    };

    pthread_create(&p, NULL, worker, &args);

    pthread_join(p, NULL);
    printf("Done.\n");

    return 0;
}
