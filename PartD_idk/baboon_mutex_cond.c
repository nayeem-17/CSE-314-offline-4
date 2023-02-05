#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

enum {UNUSED=-1, LEFT, RIGHT};
struct rope{
    int capacity ;
    int dir ; // -1, 0 , 1
    int crossing_count;
    pthread_mutex_t lock; 
    pthread_cond_t lock_cond; 
    pthread_mutex_t capacity_lock;
    pthread_cond_t capacity_cond;   
} rope ;

typedef struct {
    int id; 
    int dir ;
} args;

int baboons_left,baboons_right;
int rope_capacity ;
pthread_t *baboon_thread; 
args *baboon_thread_args;

void rope_init() {
    rope.capacity = rope_capacity;
    rope.dir = UNUSED;
    rope.crossing_count = 0;

    
    pthread_mutex_init(&rope.lock, NULL);
    pthread_cond_init(&rope.lock_cond, NULL);
    pthread_mutex_init(&rope.capacity_lock, NULL);
    pthread_cond_init(&rope.capacity_cond, NULL);
}

void *baboon(void *ptr ){
    int id= ((args*) ptr)->id; 
    int dir = ((args*) ptr)->dir; 

    pthread_mutex_lock(&rope.capacity_lock);
    while( rope.capacity <= 0 ) 
        pthread_cond_wait(&rope.capacity_cond, &rope.capacity_lock);
    rope.capacity-- ;
    pthread_mutex_unlock(&rope.capacity_lock);
    
    pthread_mutex_lock(&rope.lock);
    while( !( (rope.dir == UNUSED || rope.crossing_count == 0 || rope.dir == dir)) ) { 
        pthread_cond_wait(&rope.lock_cond,&rope.lock) ; 
    }

    rope.dir = dir;  
    rope.crossing_count++ ;
    printf("Baboon %d going %d\n", id,dir) ;
    pthread_cond_signal(&rope.lock_cond); 
    pthread_mutex_unlock(&rope.lock) ;
    
    // sleep(1) ;

    pthread_mutex_lock(&rope.lock);
    rope.crossing_count-- ;
    printf("Baboon %d reached %d\n", id,dir) ;
    if( rope.crossing_count == 0 ) rope.dir=UNUSED;
    pthread_cond_signal(&rope.lock_cond); 
    pthread_mutex_unlock(&rope.lock) ;
    
    pthread_mutex_lock(&rope.capacity_lock);
    rope.capacity++ ;
    pthread_cond_signal(&rope.capacity_cond) ;
    pthread_mutex_unlock(&rope.capacity_lock) ;
}



int main(int argc, char *argv[]){
    int i;

    if (argc < 4)
    {
        printf("./baboon #rope_capacity #baboons_left #baboons_right e.g. ./exe 10 6 4\n");
        exit(1);
    }
    else
    {
        rope_capacity = atoi(argv[1]);
        baboons_left = atoi(argv[2]);
        baboons_right = atoi(argv[3]);
        rope_init();
    }

    baboon_thread_args= (args*)malloc(sizeof(args) * (baboons_left+baboons_right));
    baboon_thread = (pthread_t *)malloc(sizeof(pthread_t) * (baboons_left+baboons_right));

    for (i = 0; i < (baboons_left); i++){
        baboon_thread_args[i].id=i;
        baboon_thread_args[i].dir = LEFT;
    }
    for (; i < (baboons_left+baboons_right); i++){
        baboon_thread_args[i].id=i;
        baboon_thread_args[i].dir = RIGHT;
    }


    for (i = 0; i < (baboons_left+baboons_right); i++){
        pthread_create(&baboon_thread[i], NULL, baboon, (void *)&baboon_thread_args[i]);
    }


    for (i = 0; i < (baboons_left+baboons_right); i++){
        pthread_join(baboon_thread[i], NULL);
        printf("Baboon %d joined\n", i);
    }

    /*----Deallocating Buffers---------------------*/
    free(baboon_thread_args);
    free(baboon_thread);

    return 0;
}