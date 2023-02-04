#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock *rw)
{
  rw->readers = 0;
  rw->writers = 0;
  rw->waiting_readers = 0;
  rw->waiting_writers = 0;
  pthread_mutex_init(&rw->lock, NULL);
  pthread_cond_init(&rw->writer_queue, NULL);
  pthread_cond_init(&rw->reader_queue, NULL);
}

void ReaderLock(struct read_write_lock *rw)
{
  pthread_mutex_lock(&rw->lock);
  rw->waiting_readers++;
  while (rw->writers || rw->waiting_writers)
  {
    pthread_cond_wait(&rw->reader_queue, &rw->lock);
  }
  rw->waiting_readers--;
  rw->readers++;
  pthread_mutex_unlock(&rw->lock);
}

void ReaderUnlock(struct read_write_lock *rw)
{
  pthread_mutex_lock(&rw->lock);
  rw->readers--;
  if (rw->readers == 0)
    pthread_cond_signal(&rw->writer_queue);
  pthread_mutex_unlock(&rw->lock);
}

void WriterLock(struct read_write_lock *rw)
{

  pthread_mutex_lock(&rw->lock);
  // if readers or active writers
  rw->waiting_writers++;
  while (rw->readers || rw->writers)
  {
    pthread_cond_wait(&rw->writer_queue, &rw->lock);
  }
  rw->waiting_writers--;
  rw->writers++;
  pthread_mutex_unlock(&rw->lock);
}

void WriterUnlock(struct read_write_lock *rw)
{
  pthread_mutex_lock(&rw->lock);
  rw->writers--;
  if (rw->waiting_writers)
  {
    pthread_cond_broadcast(&rw->writer_queue);
  }
  else
  {
    // no writers just readers or not
    pthread_cond_broadcast(&rw->reader_queue);
  }
  pthread_mutex_unlock(&rw->lock);
}
