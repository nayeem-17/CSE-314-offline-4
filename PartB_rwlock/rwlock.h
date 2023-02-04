#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

struct read_write_lock
{
    pthread_mutex_t lock;
    pthread_cond_t writer_queue;
    pthread_cond_t reader_queue;
    int readers;
    int writers;
    int waiting_readers;
    int waiting_writers;
};

void InitalizeReadWriteLock(struct read_write_lock *rw);
void ReaderLock(struct read_write_lock *rw);
void ReaderUnlock(struct read_write_lock *rw);
void WriterLock(struct read_write_lock *rw);
void WriterUnlock(struct read_write_lock *rw);
