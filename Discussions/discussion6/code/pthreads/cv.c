#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

char *input;
int ready = 0;
pthread_mutex_t lock;
pthread_cond_t cond;

void* worker(void* arg) {
        
    //allocate memory for input     
    pthread_mutex_lock(&lock);      
    unsigned int seconds = 3;
    sleep(seconds);                  
    input = (char*)malloc(30);
    ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    //initialize pthread_mutex_t variable        
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    
    //create a thread to run worker function            
    pthread_t p;    
    pthread_create(&p, NULL, worker, NULL);
    
    //wait for condition    
    pthread_mutex_lock(&lock); 
    while(ready == 0){
            printf("wait\n");
            pthread_cond_wait(&cond, &lock);
    }    
    printf("your input>> ");    
    fgets(input, 50, stdin);
    pthread_mutex_unlock(&lock);
    
    printf("%s", input);
    
    return 0;
}


