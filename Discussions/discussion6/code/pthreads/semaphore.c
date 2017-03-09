#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


int balance = 0;
//define semaphore variable
sem_t s;

void* worker(void* arg) {
        
    //increase balance by 1        
    sem_wait(&s);                   
    balance++; 
    sem_post(&s); 
    
    return NULL;
}

int main(int argc, char *argv[]) {
    //initialize semaphore variable        
    sem_init(&s, 0, 1);
    
    //create a thread to run worker function            
    pthread_t p;    
    pthread_create(&p, NULL, worker, NULL);
    
    //increase balance by 1
    sem_wait(&s);
    balance++; 
    sem_post(&s);  
        
    //wait for thread p to complete        
    pthread_join(p, NULL);
    
    //print the balance
    printf("balance is %d.\n", balance);
    
    return 0;
}
