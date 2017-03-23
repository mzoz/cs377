#include "ProducerConsumer.h"
#include <atomic>
#include <thread>      
#include <chrono>
#include <sys/time.h>

//TODO: add BoundedBuffer, locks and any global variables here

std::atomic<int> items_added(0); //incremented when item is added
std::atomic<int> items_consumed(0); //incremented when item is consumed

int max_items; //unchanging max items
BoundedBuffer *bb = new BoundedBuffer(10); 
ofstream fout;

pthread_mutex_t write_lock; //write lock for fout
int _CSLEEP_, _PSLEEP_; // const consumer, producer ms sleep values

double time_start = 0;

#define DEBUG 0


void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//init globals
	max_items = items;
	_CSLEEP_ = csleep;
	_PSLEEP_ = psleep;

	pthread_mutex_init(&write_lock, NULL);
	pthread_t producer_threads[p];
	pthread_t consumer_threads[c];

	fout.open("output.txt");

	struct timeval t;
    gettimeofday(&t, NULL);
    time_start = (double) ((double)t.tv_sec + (double)t.tv_usec / 1e6);

	//create producer threads before consumer threads
	for (int pt_index = 0; pt_index < p; pt_index++) 
		pthread_create(&producer_threads[pt_index], NULL, producer, (void *) (intptr_t) pt_index);

	for (int ct_index = 0; ct_index < c; ct_index++) 
		pthread_create(&consumer_threads[ct_index], NULL, consumer, (void *) (intptr_t) ct_index);


	//join producer threads before consumer threads
	for (int pt_index = 0; pt_index < p; pt_index ++)
		pthread_join(producer_threads[pt_index], NULL);

	for (int ct_index = 0; ct_index < c; ct_index ++)
		pthread_join(consumer_threads[ct_index], NULL);

	fout.close();

}


void* producer(void* threadID){
	int tid = (intptr_t) (threadID);

	int item_num;
	//exit condition, items added == max_items
	while((item_num = items_added.fetch_add(1)) < max_items){
		
		//sleep for __PSLEEP_ ms
		std::this_thread::sleep_for (std::chrono::milliseconds(_PSLEEP_));
		
		// generate random item to add
		int item_to_add  = rand() % 10 + 1;

		#if DEBUG
		printf("[PT %d] bb->append(%d);\n", tid, item_to_add);
		#endif

		//add the item
		bb->append(item_to_add);


		struct timeval t;
    	gettimeofday(&t, NULL);
    	double time_cur = (double) ((double)t.tv_sec + (double)t.tv_usec / 1e6);

    	char time_str[50];

		sprintf ( time_str, "%.2f sec", time_cur - time_start );

		//acquire write lock
		pthread_mutex_lock(&write_lock);

		//write to file
		fout << "Producer #" << tid << ", time = " << time_str << ", producing data item #" 
			<< item_num << ", item value=" << item_to_add << endl;

		//unlock write mutex
		pthread_mutex_unlock(&write_lock);

	}

	#if DEBUG
		printf("[PT %d] exiting\n", tid);
	#endif

	return NULL;
}

void* consumer(void* threadID){
	int tid = (intptr_t) threadID;

	//exit condition: items consumed == max items
	while(items_consumed.fetch_add(1) < max_items){

		//first sleep for CSLEEP ms
		std::this_thread::sleep_for (std::chrono::milliseconds(_CSLEEP_));

		// remove item
		int consumed_item = bb->remove(); // will block if no items on buf

		#if DEBUG
		printf("[CT %d] consumed: %d;\n", tid, consumed_item);
		#endif

		//acq write lock
		pthread_mutex_lock(&write_lock);

		struct timeval t;
    	gettimeofday(&t, NULL);
    	double time_cur = (double) ((double)t.tv_sec + (double)t.tv_usec / 1e6);

    	char time_str[50];

		sprintf ( time_str, "%.2f sec", time_cur - time_start );

		//write output
		fout << "Consumer #" << tid << 
			", time = " << time_str << ", consuming data item with value=" 
				<< consumed_item << endl;

		//unlock mutex
		pthread_mutex_unlock(&write_lock);
	}

	#if DEBUG
		printf("[CT %d] exiting\n", tid);
	#endif

	return NULL;
}
