#ifndef FS_L
#define FS_L
#include <pthread.h>

/* 
 * To avoid code repetition, we just grouped up most of the 
 * functions we use to work with locks and plopped them into 
 * this file, which we imported into our other files.
 */

/* 
 * LOCK_ARRAY_SIZE = MAX_INPUT_SIZE/2, hence 50
 * We did this because since each directory must be, at least, something like
 * "a/", which takes up 2 chars, the max number of inodes that need to be
 * locked will be, at most, MAX_INPUT_SIZE / (number of characters each 
 * directory takes up), which will be no higher than 50 in our case.
 */

#define LOCK_ARRAY_SIZE 50

/* Array of pointers to locks, with corresponding size of the array */
typedef struct lockArray
{
    int size;
    pthread_rwlock_t *array[LOCK_ARRAY_SIZE];
} lockArray;

/* 
 * Collection of functions that facilitate working with rwlocks 
 */

void rdlock(pthread_rwlock_t *lock);
void wrlock(pthread_rwlock_t *lock);
void unlock(pthread_rwlock_t *lock);
void unlockArray(lockArray locks);
void mutexLock(pthread_mutex_t *lock);
void mutexUnlock(pthread_mutex_t *lock);
int tryWrlock(pthread_rwlock_t *lock);

#endif /* FS_L */
