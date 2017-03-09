#include <stdio.h>
#include <pthread.h>

int balance = 0;

// define mutex variable
pthread_mutex_t lock;

void* worker(void* arg) {

    //increase balance by 1
    pthread_mutex_lock(&lock);
    balance++;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char *argv[]) {
    //initialize pthread_mutex_t variable
    pthread_mutex_init(&lock, NULL);

    //create a thread to run worker function
    pthread_t p;
    pthread_create(&p, NULL, worker, NULL);

    //increase balance by 1
    pthread_mutex_lock(&lock);
    balance++;
    pthread_mutex_unlock(&lock);

    //wait for thread p to complete
    pthread_join(p, NULL);

    //print the balance
    printf("balance is %d.\n", balance);

    return 0;
}
