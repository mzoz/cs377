#include "BoundedBuffer.h"
#include <pthread.h>



BoundedBuffer::BoundedBuffer(int N){
	pthread_cond_init(&buffer_full, NULL);
	pthread_cond_init(&buffer_empty, NULL);
	pthread_mutex_init(&buffer_lock, NULL);
	buffer = new int[N];
	buffer_size = N; // total size of buffer DO NOT CHANGE
	buffer_cnt = 0; //total number of items in the buffer
	buffer_last = 0; //buffer last will point to the NEXT index in this implementation
}


void BoundedBuffer::append(int data){
	pthread_mutex_lock(&buffer_lock);

	while(buffer_cnt == buffer_size){
		pthread_cond_wait(&buffer_full, &buffer_lock);
	}

	buffer[buffer_last] = data;
	buffer_cnt ++;
	buffer_last = (buffer_last + 1) % buffer_size;

	pthread_cond_signal(&buffer_empty); 
	//signal to remove threads buffer has item to remove

	pthread_mutex_unlock(&buffer_lock);
}

int BoundedBuffer::remove(){
	pthread_mutex_lock(&buffer_lock);

	while(buffer_cnt == 0){
		pthread_cond_wait(&buffer_empty, &buffer_lock);
		// wait on buffer_empty condition until something fills it
	}

	//calculate correct index of head to remove from
	int head_index = (abs(buffer_last - buffer_cnt) % buffer_size);

	int removed_item = buffer[head_index];

	buffer_cnt --;

	//signal that item has been added to threads waiting on append
	pthread_cond_signal(&buffer_full);

	pthread_mutex_unlock(&buffer_lock);

	return removed_item;
}

bool BoundedBuffer::isEmpty(){
	return buffer_cnt == 0;
}
