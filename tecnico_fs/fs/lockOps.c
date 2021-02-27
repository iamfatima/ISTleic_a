#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "lockOps.h"

/* Locks a rwlock in read mode, checking for errors */
void rdlock(pthread_rwlock_t *lock)
{
    if (pthread_rwlock_rdlock(lock))
    {
        fprintf(stderr, "Problem locking rwlock in read mode.\n");
        exit(EXIT_FAILURE);
    }
}

/* Locks a rwlock in write mode, checking for errors */
void wrlock(pthread_rwlock_t *lock)
{
    if (pthread_rwlock_wrlock(lock))
    {
        fprintf(stderr, "Problem locking rwlock in write mode.\n");
        exit(EXIT_FAILURE);
    }
}

/* Unlocks a rwlock, checking for errors */
void unlock(pthread_rwlock_t *lock)
{
    if (pthread_rwlock_unlock(lock))
    {
        fprintf(stderr, "Problem unlocking rwlock.\n");
        exit(EXIT_FAILURE);
    }
}

/* Unlocks a lockArray of size "size", checking for errors */
void unlockArray(lockArray locks)
{
    for (size_t i = 0; i < locks.size; i++)
    {
        unlock(locks.array[i]);
    }
}

/* 
 * Tries to lock a rwlock, checking for erros 
 * Output:
 *  - 0 if everything is ok.
 *  - EBUSY if the lock is occupied.
 */
int tryWrlock(pthread_rwlock_t *lock)
{
    int err = pthread_rwlock_trywrlock(lock);
    if (err)
    {
        if (err == EBUSY)
        {
            return err;
        }
        else
        {
            fprintf(stderr, "Problem locking rwlock. Error: %d\n", err);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

/* Locks a mutex, checking for errors */
void mutexLock(pthread_mutex_t *lock)
{
    if (pthread_mutex_lock(lock))
    {
        fprintf(stderr, "Problem locking mutex.\n");
        exit(EXIT_FAILURE);
    }
}

/* Unlocks a mutex, checking for errors */
void mutexUnlock(pthread_mutex_t *lock)
{
    if (pthread_mutex_unlock(lock))
    {
        fprintf(stderr, "Problem unlocking mutex.\n");
        exit(EXIT_FAILURE);
    }
}
