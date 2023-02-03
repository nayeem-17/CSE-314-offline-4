#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock *rw)
{
  //	Write the code for initializing your read-write lock.
  pthread_mutex_init(&rw->lock, NULL);
  pthread_mutex_init(&rw->readerlock, NULL);
  rw->readers = 0;
  rw->writers = 0;
}

// void ReaderLock(struct read_write_lock *rw)
// {
//   //	Write the code for aquiring read-write lock by the reader.
//   pthread_mutex_lock(&rw->lock);
//   while (rw->writers > 0)
//   {
//     pthread_mutex_unlock(&rw->lock);
//     pthread_yield();
//     pthread_mutex_lock(&rw->lock);
//   }
//   rw->readers++;
//   pthread_mutex_unlock(&rw->lock);
// }

// void ReaderUnlock(struct read_write_lock *rw)
// {
//   //	Write the code for releasing read-write lock by the reader.
//   pthread_mutex_lock(&rw->lock);
//   rw->readers--;
//   pthread_mutex_unlock(&rw->lock);
// }

// void WriterLock(struct read_write_lock *rw)
// {
//   //	Write the code for aquiring read-write lock by the writer.
//   pthread_mutex_lock(&rw->writelock);
//   rw->writers++;
//   while (rw->readers > 0)
//   {
//     pthread_mutex_unlock(&rw->writelock);
//     pthread_yield();
//     pthread_mutex_lock(&rw->writelock);
//   }
//   pthread_mutex_lock(&rw->lock);
//   pthread_mutex_unlock(&rw->writelock);
// }

// void WriterUnlock(struct read_write_lock *rw)
// {
//   //	Write the code for releasing read-write lock by the writer.
//   pthread_mutex_lock(&rw->lock);
//   pthread_mutex_unlock(&rw->writelock);
//   rw->writers--;
//   pthread_mutex_unlock(&rw->lock);
// }

// void WriterLock(struct read_write_lock *rw)
// {
//   //	Write the code for aquiring read-write lock by the reader.
//   pthread_mutex_lock(&rw->lock);
//   rw->writers++;
//   if (rw->writers == 1)
//     pthread_mutex_lock(&rw->readerlock);
//   pthread_mutex_unlock(&rw->lock);
// }

// void WriterUnlock(struct read_write_lock *rw)
// {
//   //	Write the code for releasing read-write lock by the reader.
//   pthread_mutex_lock(&rw->lock);
//   rw->writers--;
//   if (rw->writers == 0)
//     pthread_mutex_unlock(&rw->readerlock);
//   pthread_mutex_unlock(&rw->lock);
// }

// void ReaderLock(struct read_write_lock *rw)
// {
//   //	Write the code for aquiring read-write lock by the writer.
//   pthread_mutex_lock(&rw->readerlock);
// }

// void ReaderUnlock(struct read_write_lock *rw)
// {
//   //	Write the code for releasing read-write lock by the writer.
//   pthread_mutex_unlock(&rw->readerlock);
// }
