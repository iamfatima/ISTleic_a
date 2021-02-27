#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "state.h"
#include "../tecnicofs-api-constants.h"
#include "lockOps.h"

inode_t inode_table[INODE_TABLE_SIZE];

/*
 * Locks root i-node in write mode.
 */
void lock_root()
{
    wrlock(&inode_table[FS_ROOT].lock);
}

/*
 * Unlocks root i-node.
 */
void unlock_root()
{
    unlock(&inode_table[FS_ROOT].lock);
}

/*
 * Sleeps for synchronization testing.
 */
void insert_delay(int cycles)
{
    for (int i = 0; i < cycles; i++)
    {
    }
}

/*
 * Initializes the i-nodes table.
 */
void inode_table_init()
{
    for (int i = 0; i < INODE_TABLE_SIZE; i++)
    {
        inode_table[i].nodeType = T_NONE;
        inode_table[i].data.dirEntries = NULL;
        inode_table[i].data.fileContents = NULL;

        if (pthread_rwlock_init(&inode_table[i].lock, NULL))
        {
            fprintf(stderr, "Problem initializing tecnicofs locks.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * Releases the allocated memory for the i-nodes tables.
 */
void inode_table_destroy()
{
    for (int i = 0; i < INODE_TABLE_SIZE; i++)
    {
        if (inode_table[i].nodeType != T_NONE)
        {
            /* 
             * As data is an union, the same pointer is used for both dirEntries 
             * and fileContents just release one of them 
             */
            if (inode_table[i].data.dirEntries)
                free(inode_table[i].data.dirEntries);

            if (pthread_rwlock_destroy(&inode_table[i].lock))
            {
                fprintf(stderr, "Problem destroying tecnicofs locks.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/*
 * Creates a new i-node in the table with the given 
 * information, locking its lock in write mode.
 * Input:
 *  - nType: the type of the node (file or directory)
 *  - lock: pointer to a pointer to a 
 * Returns:
 *  inumber: identifier of the new i-node, if successfully created
 *     FAIL: if an error occurs
 */
int inode_create(type nType, pthread_rwlock_t **lock)
{
    /* Used for testing synchronization speedup */
    insert_delay(DELAY);

    for (int inumber = 0; inumber < INODE_TABLE_SIZE; inumber++)
    {
        /* 
         * Since we call inode_create() from create(), we need to account for
         * all the locks we already have set. We can't try to simply lock the
         * i-nodes, since that would result in some nasty deadlocks.
         * So, we try to lock each one and move along if it's already locked.
         * This way, there is a chance we cycle through all the i-nodes and miss
         * one that was being deleted and that would soon be free, but it also
         * deals with the i-nodes which are locked by the thread that calls
         * inode_create(), avoiding deadlocks.
         * NOTE: if an i-node is locked, it's not empty and therefore not 
         * available to be initialized. 
         */
        if (tryWrlock(&inode_table[inumber].lock))
        {
            continue;
        }

        if (inode_table[inumber].nodeType == T_NONE)
        {
            inode_table[inumber].nodeType = nType;

            if (nType == T_DIRECTORY)
            {
                /* Initializes entry table */
                inode_table[inumber].data.dirEntries = malloc(sizeof(DirEntry) * MAX_DIR_ENTRIES);

                for (int i = 0; i < MAX_DIR_ENTRIES; i++)
                {
                    inode_table[inumber].data.dirEntries[i].inumber = FREE_INODE;
                }
            }
            else
            {
                inode_table[inumber].data.fileContents = NULL;
            }

            /* Returns the pointer to the i-node lock to the variable "lock" */
            *lock = &inode_table[inumber].lock;

            return inumber;
        }

        unlock(&inode_table[inumber].lock);
    }
    return FAIL;
}

/*
 * Deletes the i-node.
 * Input:
 *  - inumber: identifier of the i-node
 * Returns: SUCCESS or FAIL
 */
int inode_delete(int inumber)
{
    /* Used for testing synchronization speedup */
    insert_delay(DELAY);

    if ((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].nodeType == T_NONE))
    {
        printf("inode_delete: invalid inumber\n");
        return FAIL;
    }

    /* 
     * The i-node's lock is already locked by delete(), 
     * so we don't need to lock anything 
     */

    inode_table[inumber].nodeType = T_NONE;

    /* see inode_table_destroy function */
    if (inode_table[inumber].data.dirEntries)
        free(inode_table[inumber].data.dirEntries);

    return SUCCESS;
}

/*
 * Copies the contents of the i-node into the arguments.
 * Only the fields referenced by non-null arguments are copied.
 * Locks the inode in read mode.
 * Input:
 *  - inumber: identifier of the i-node
 *  - nType: pointer to type
 *  - data: pointer to data
 *  - lock: pointer to lock
 *  - lock_mode: rwlock lock mode - 'r' for read, 'w' for write, 
 *  't' for trylock in write mode and 'n' for no lock
 *  - verify_lock: 'v' to verify against a lockedLock, 'n' not to do that
 *  - lockLock: pointer to a lock to be checked against
 * Returns: 
 *  - SUCCESS: If everything went ok
 *  - FAIL: If something went wrong
 *  - FAILTRY: if the trylock failed
 *  - VERIFYSUCCESS: if our lock was the same as lockedLock
 */
int inode_get(int inumber, type *nType, union Data *data, pthread_rwlock_t **lock, char lock_mode, char verify_lock, pthread_rwlock_t *lockedLock)
{
    /* Used for testing synchronization speedup */
    insert_delay(DELAY);

    /* 
     * If we have verify_lock on, we check the lock 
     * we want to lock against a lockedLock. 
     * If they are the same, we don't lock ours.
     */
    if (verify_lock == 'v' && &inode_table[inumber].lock == lockedLock)
    {
        //No lock
    }
    else if (lock_mode == 'r')
    {
        rdlock(&inode_table[inumber].lock);
    }
    else if (lock_mode == 'w')
    {
        wrlock(&inode_table[inumber].lock);
    }
    else if (lock_mode == 't')
    {
        if (tryWrlock(&inode_table[inumber].lock))
        {
            return FAILTRY;
        }
    }
    else if (lock_mode == 'n')
    {
        //No lock
    }
    else
    {
        printf("inode_get: invalid lock mode '%c'\n", lock_mode);
        return FAIL;
    }

    if ((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].nodeType == T_NONE))
    {
        printf("inode_get: invalid inumber %d\n", inumber);

        /* 
         * As long as lock_mode isn't n 
         * OR verify_lock is on 
         * AND the pointer to a locked lock is the same as our pointer, 
         * we locked something and we have to unlock it.
         */
        if (lock_mode != 'n' || (verify_lock == 'v' && &inode_table[inumber].lock == lockedLock))
        {
            unlock(&inode_table[inumber].lock);
        }
        return FAIL;
    }

    if (nType)
        *nType = inode_table[inumber].nodeType;

    if (data)
        *data = inode_table[inumber].data;

    /* There's always a lock */
    *lock = &inode_table[inumber].lock;

    if (verify_lock == 'v' && &inode_table[inumber].lock == lockedLock)
    {
        return VERIFYSUCCESS;
    }

    return SUCCESS;
}

/*
 * Resets an entry for a directory.
 * Input:
 *  - inumber: identifier of the i-node
 *  - sub_inumber: identifier of the sub i-node entry
 * Returns: SUCCESS or FAIL
 */
int dir_reset_entry(int inumber, int sub_inumber)
{
    /* Used for testing synchronization speedup */
    insert_delay(DELAY);

    if ((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].nodeType == T_NONE))
    {
        printf("inode_reset_entry: invalid inumber\n");
        return FAIL;
    }

    if (inode_table[inumber].nodeType != T_DIRECTORY)
    {
        printf("inode_reset_entry: can only reset entry to directories\n");
        return FAIL;
    }

    if ((sub_inumber < FREE_INODE) || (sub_inumber > INODE_TABLE_SIZE) || (inode_table[sub_inumber].nodeType == T_NONE))
    {
        printf("inode_reset_entry: invalid entry inumber\n");
        return FAIL;
    }

    for (int i = 0; i < MAX_DIR_ENTRIES; i++)
    {
        if (inode_table[inumber].data.dirEntries[i].inumber == sub_inumber)
        {
            inode_table[inumber].data.dirEntries[i].inumber = FREE_INODE;
            inode_table[inumber].data.dirEntries[i].name[0] = '\0';
            return SUCCESS;
        }
    }
    return FAIL;
}

/*
 * Adds an entry to the i-node directory data.
 * Input:
 *  - inumber: identifier of the i-node
 *  - sub_inumber: identifier of the sub i-node entry
 *  - sub_name: name of the sub i-node entry 
 * Returns: SUCCESS or FAIL
 */
int dir_add_entry(int inumber, int sub_inumber, char *sub_name)
{
    /* Used for testing synchronization speedup */
    insert_delay(DELAY);

    if ((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].nodeType == T_NONE))
    {
        printf("inode_add_entry: invalid inumber\n");
        return FAIL;
    }

    if (inode_table[inumber].nodeType != T_DIRECTORY)
    {
        printf("inode_add_entry: can only add entry to directories\n");
        return FAIL;
    }

    if ((sub_inumber < 0) || (sub_inumber > INODE_TABLE_SIZE) || (inode_table[sub_inumber].nodeType == T_NONE))
    {
        printf("inode_add_entry: invalid entry inumber\n");
        return FAIL;
    }

    if (strlen(sub_name) == 0)
    {
        printf("inode_add_entry: \
               entry name must be non-empty\n");
        return FAIL;
    }

    for (int i = 0; i < MAX_DIR_ENTRIES; i++)
    {
        if (inode_table[inumber].data.dirEntries[i].inumber == FREE_INODE)
        {
            inode_table[inumber].data.dirEntries[i].inumber = sub_inumber;
            strcpy(inode_table[inumber].data.dirEntries[i].name, sub_name);
            return SUCCESS;
        }
    }
    return FAIL;
}

/*
 * Prints the i-nodes table.
 * Input:
 *  - inumber: identifier of the i-node
 *  - name: pointer to the name of current file/dir
 */
void inode_print_tree(FILE *fp, int inumber, char *name)
{
    if (inode_table[inumber].nodeType == T_FILE)
    {
        fprintf(fp, "%s\n", name);
        return;
    }

    if (inode_table[inumber].nodeType == T_DIRECTORY)
    {
        fprintf(fp, "%s\n", name);
        for (int i = 0; i < MAX_DIR_ENTRIES; i++)
        {
            if (inode_table[inumber].data.dirEntries[i].inumber != FREE_INODE)
            {
                char path[MAX_FILE_NAME];
                if (snprintf(path, sizeof(path), "%s/%s", name, inode_table[inumber].data.dirEntries[i].name) > sizeof(path))
                {
                    fprintf(stderr, "truncation when building full path\n");
                }
                inode_print_tree(fp, inode_table[inumber].data.dirEntries[i].inumber, path);
            }
        }
    }
}
