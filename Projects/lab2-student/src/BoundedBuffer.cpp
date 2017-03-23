#include "BoundedBuffer.h"
#include <pthread.h>



BoundedBuffer::BoundedBuffer(int N){
	pthread_cond_init(&buffer_full, NULL);
	pthread_cond_init(&buffer_empty, NULL);
	pthread_mutex_init(&buffer_lock, NULL);
	buffer = new int[N]();
	buffer_size = N; // total size of buffer DO NOT CHANGE
	buffer_cnt = 0; //total number of items in the buffer
	buffer_last = N - 1; //buffer last will point to the last item added
}

#if DEBUG

void BoundedBuffer::print_buffer(){
	printf("\nBuffer : ");
	for(int i = 0; i < buffer_size; i ++){
		printf(" |%d| ",buffer[i]);
	}
	printf("\n\n");
}

#endif

void BoundedBuffer::append(int data){
	pthread_mutex_lock(&buffer_lock);

	while(buffer_cnt == buffer_size){
		pthread_cond_wait(&buffer_full, &buffer_lock);
	}

	buffer_last = (buffer_last + 1) % (buffer_size);

	buffer[buffer_last] = data;
	buffer_cnt ++;


	#if DEBUG

	printf("\n\tBoundedBuffer::append()[62:44] print_buffer: \n");

	this->print_buffer();

	printf("\n\tBoundedBuffer::append(), data:%d, buffer_cnt: %d, buffer_last: %d, appended: %d\n",
			data, buffer_cnt, buffer_last, buffer[buffer_last]);

	#endif

	pthread_cond_signal(&buffer_empty); 
	//signal to remove threads buffer has item to remove

	pthread_mutex_unlock(&buffer_lock);
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int BoundedBuffer::remove(){
	pthread_mutex_lock(&buffer_lock);

	while(buffer_cnt == 0){
		pthread_cond_wait(&buffer_empty, &buffer_lock);
		// wait on buffer_empty condition until something fills it
	}

	//calculate correct index of head to remove from
	int head_index =  mod(buffer_last - (buffer_cnt - 1), buffer_size);
							
	// if buffer_last == 0, take buffer_size(1+ max index position), subtract head delta

	int removed_item = buffer[head_index];

#if DEBUG

	//assert(head_index < buffer_cnt)

	printf("\n\tBoundedBuffer::remove()[62:44] print_buffer: \n");

	this->print_buffer();

	printf("\n\tBoundedBuffer::remove(), buffer_cnt: %d, buffer_last: %d, head_index: %d, removed_item: %d\n",
			buffer_cnt, buffer_last, head_index, buffer[head_index]);

#endif

	buffer_cnt --;

	//signal that item has been removed to threads waiting on append
	pthread_cond_signal(&buffer_full);

	pthread_mutex_unlock(&buffer_lock);

	return removed_item;
}

bool BoundedBuffer::isEmpty(){
	return buffer_cnt == 0;
}
