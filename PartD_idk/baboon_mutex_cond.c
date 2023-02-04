#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


enum {UNUSED=-1, LEFT, RIGHT};

struct rope{
    int capacity ;
    int dir ; // -1, 0 , 1
    int cnt;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  lock_cond = PTHREAD_COND_INITIALIZER; 
    pthread_mutex_t wait = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  wait_cond = PTHREAD_COND_INITIALIZER;
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
    rope.cnt = 0;

    // zem_init(&rope.lock, rope.capacity);
    // zem_init(&rope.wait, 1);
    // zem_init(&rope.baboons, rope.capacity) ;
}

void acquire_rope_lock(){
    rope.cnt++ ;
    zem_down(&rope.lock);
}
void release_rope_lock(){
    rope.cnt-- ;
    zem_up(&rope.lock);
}

void *baboon(void *ptr ){
    int id= ((args*) ptr)->id; 
    int dir = ((args*) ptr)->dir; 

    while(1) {
        acquire_rope_lock() ;
        if( !( (rope.dir == UNUSED || rope.cnt == 1 || rope.dir == dir)) ) { //&& rope.cnt < rope.capacity 
            release_rope_lock() ;
            zem_down(&rope.wait) ;
            continue; 
        }

        rope.dir = dir;  
        printf("Baboon %d going %d\n", id,dir) ;
        // printf()
        zem_up(&rope.wait) ;
        release_rope_lock();
        break; 
    }

   
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