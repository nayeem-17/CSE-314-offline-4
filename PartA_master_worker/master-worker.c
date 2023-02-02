#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

int item_to_produce,item_to_consume, curr_buf_size;
int total_items,consumed_items_count, max_buf_size, num_workers, num_masters;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int *buffer;
const int FREE=-1; 

void print_produced(int num, int master)
{
    printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker)
{
    printf("Consumed %d by worker %d\n", num, worker);
}

// find free item in buffer 
int find_free(){
    for(int i=0;i<max_buf_size;i++){
        if( buffer[i] == FREE ) return i ;
    }
    return -1; 
}
int find_used(){
    for(int i=0;i<max_buf_size;i++){
        if( buffer[i] != FREE) return i ;
    }
    return -1;

}

// produce items and place in buffer
// modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
    int thread_id = *((int *)data);
    int buffer_index;
    int endmaster=0; 
    while (1)
    {
        pthread_mutex_lock(&lock) ;
        
        while( (buffer_index=find_free()) == -1 && item_to_produce < total_items ){
            pthread_cond_wait(&cond, &lock) ;
        }
        if( item_to_produce < total_items ) {
            buffer[buffer_index] = item_to_produce;
            print_produced(item_to_produce, thread_id);
            item_to_produce++;
        }
        else endmaster=1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock) ;
        if( endmaster ) break; 
    }
    return 0;
}

// write function to be run by worker threads
// ensure that the workers call the function print_consumed when they consume an item
void *consume_requests_loop(void *data) {
    int thread_id = *((int *)data);
    int buffer_index;
    int endconsumer=0;
    while (1)
    {
        pthread_mutex_lock(&lock) ;
        
        while( (buffer_index=find_used()) == -1 && consumed_items_count < total_items ){
            pthread_cond_wait(&cond, &lock) ;
        }
        if( consumed_items_count < total_items ) {
            item_to_consume = buffer[buffer_index] ;
            buffer[buffer_index]=FREE; 
            print_consumed(item_to_consume, thread_id);
            consumed_items_count++;
        }
        else endconsumer=1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock) ;
        if( endconsumer ) break ;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int *master_thread_id, *worker_thread_id;
    pthread_t *master_thread, *worker_thread;
    item_to_produce = 0;
    curr_buf_size = 0;
    consumed_items_count=0; 
    
    int i;

    if (argc < 5)
    {
        printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
        exit(1);
    }
    else
    {
        num_masters = atoi(argv[4]);
        num_workers = atoi(argv[3]);
        total_items = atoi(argv[1]);
        max_buf_size = atoi(argv[2]);
    }

    buffer = (int *)malloc(sizeof(int) * max_buf_size);
    for(i=0;i<max_buf_size;i++)
        buffer[i]=FREE; 
    // create master producer threads
    master_thread_id = (int *)malloc(sizeof(int) * num_masters);
    master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
    worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
    worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);

    for (i = 0; i < num_masters; i++)
        master_thread_id[i] = i;
    for (i = 0; i < num_workers; i++)
        worker_thread_id[i] = i;

    for (i = 0; i < num_masters; i++)
        pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
    for(i=0;i<num_workers;i++)
        pthread_create(&worker_thread[i], NULL, consume_requests_loop, (void *)&worker_thread_id[i]);

    // wait for all threads to complete
    for (i = 0; i < num_masters; i++)
    {
        pthread_join(master_thread[i], NULL);
        printf("master %d joined\n", i);
    }
    for(i=0;i<num_workers;i++){
        pthread_join(worker_thread[i], NULL) ;
        printf("worker %d joined\n",i ) ;
    }

    /*----Deallocating Buffers---------------------*/
    free(buffer);
    free(master_thread_id);
    free(master_thread);
    free(worker_thread_id);
    free(worker_thread) ;

    return 0;
}
