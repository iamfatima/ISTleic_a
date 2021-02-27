#include "operations.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "lockOps.h"

/* 
 * Given a path, fills pointers with strings for the parent path and child
 * file name
 * Input:
 *  - path: the path to split. ATENTION: the function may alter this parameter
 *  - parent: reference to a char*, to store parent path
 *  - child: reference to a char*, to store child file name
 */
void split_parent_child_from_path(char *path, char **parent, char **child)
{

	int n_slashes = 0, last_slash_location = 0;
	int len = strlen(path);

	// deal with trailing slash ( a/x vs a/x/ )
	if (path[len - 1] == '/')
	{
		path[len - 1] = '\0';
	}

	for (int i = 0; i < len; ++i)
	{
		if (path[i] == '/' && path[i + 1] != '\0')
		{
			last_slash_location = i;
			n_slashes++;
		}
	}

	if (n_slashes == 0)
	{ // root directory
		*parent = "";
		*child = path;
		return;
	}

	path[last_slash_location] = '\0';
	*parent = path;
	*child = path + last_slash_location + 1;
}

/*
 * Checks to see if a given path2 is a subpath of path1
 * Input:
 *  - path: string with the first path
 *  - subpath: string with the possible subpath
 * Output:
 *  - SUCCESS: if path1 is contained in path2
 *  - FAIL: otherwise
 */
int is_path_subpath(char *path, char *subpath)
{
	int pathLen = strlen(path);
	int subpathLen = strlen(subpath);
	char path_copy[MAX_FILE_NAME], subpath_copy[MAX_FILE_NAME];

	strcpy(path_copy, path);
	strcpy(subpath_copy, subpath);

	/* We cut the second string so it has as many chars as the first path */
	subpath_copy[pathLen] = '\0';

	/* 
	 * If the second path is bigger than the first one AND 
	 * starts with the first one, then path2 is inside path1
	 */
	if (subpathLen > pathLen && !strcmp(path_copy, subpath_copy))
	{
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

/*
 * Takes in a path and gets its depth
 * Input:
 *  - path: string conataining a path
 * Output:
 *  - depth: depth at which is node is held
 */
int get_path_depth(char *path)
{
	char full_path[MAX_FILE_NAME];
	char delim[] = "/";
	int depth = 0;

	strcpy(full_path, path);

	char *saveptr;
	char *current_path = strtok_r(full_path, delim, &saveptr);

	while (current_path != NULL)
	{
		depth++;
		current_path = strtok_r(NULL, delim, &saveptr);
	}

	return depth;
}

/*
 * Initializes tecnicofs and creates root node.
 */
void init_fs()
{
	inode_table_init();
	pthread_rwlock_t *lockPointer;

	/* create root inode */
	int root = inode_create(T_DIRECTORY, &lockPointer);

	/* inode_create() locks the i-node in write mode, so we have to unlock it */
	unlock(lockPointer);

	if (root != FS_ROOT)
	{
		printf("failed to create node for tecnicofs root\n");
		exit(EXIT_FAILURE);
	}
}

/*
 * Destroy tecnicofs and inode table.
 */
void destroy_fs()
{
	inode_table_destroy();
}

/*
 * Checks if content of directory is not empty.
 * Input:
 *  - entries: entries of directory
 * Returns: SUCCESS or FAIL
 */

int is_dir_empty(DirEntry *dirEntries)
{
	if (dirEntries == NULL)
	{
		return FAIL;
	}
	for (int i = 0; i < MAX_DIR_ENTRIES; i++)
	{
		if (dirEntries[i].inumber != FREE_INODE)
		{
			return FAIL;
		}
	}
	return SUCCESS;
}

/*
 * Looks for node in directory entry from name.
 * Input:
 *  - name: path of node
 *  - entries: entries of directory
 * Returns:
 *  - inumber: found node's inumber
 *  - FAIL: if not found
 */
int lookup_sub_node(char *name, DirEntry *entries)
{
	if (entries == NULL)
	{
		return FAIL;
	}
	for (int i = 0; i < MAX_DIR_ENTRIES; i++)
	{
		if (entries[i].inumber != FREE_INODE && strcmp(entries[i].name, name) == 0)
		{
			return entries[i].inumber;
		}
	}
	return FAIL;
}

/*
 * Lookup for a given path, locking every inumber up to that inode
 * Input:
 *  - name: path of node
 *  - locks: empty array of locks and its size, to be filled with locked inodes
 *  - lock_mode: either 'r' for read mode, 'w' for write mode and 't' for trylock
 *  in write mode
 *  - verify_mode: 'v' for verify all locks against lockedLock or 'n' to not
 *  - lockedLock: a pointer to a locked lock we want to avoid locking
 * Returns:
 *  inumber: identifier of the i-node, if found
 *     FAILTRY: if the trylock failed
 *     FAIL: otherwise
 */
int get_inumber(char *name, lockArray *locks, char lock_mode, char verify_lock, pthread_rwlock_t *lockedLock)
{
	char full_path[MAX_FILE_NAME];
	char delim[] = "/";
	int i = 0;

	/* Used to store the output of the last loop */
	int last_output;

	strcpy(full_path, name);

	/* start at root node */
	int current_inumber = FS_ROOT;

	/* use for copy */
	type nType;
	union Data data;

	char *saveptr;
	char *path = strtok_r(full_path, delim, &saveptr);

	/* 
	 * If path is already NULL, that means we have the parent directory and
	 * we should lock it in whichever lock_mode we specified.
	 */
	if (path == NULL)
	{
		/* 
		 * If verify_locked succeeded, then the lock wasn't locked, 
		 * so we take it out from the array 
		 */
		if (inode_get(current_inumber, &nType, &data, &(locks->array[i++]), lock_mode, verify_lock, lockedLock) == VERIFYSUCCESS)
		{
			i--;
		}
	}
	else
	{
		if (inode_get(current_inumber, &nType, &data, &(locks->array[i++]), 'r', verify_lock, lockedLock) == VERIFYSUCCESS)
		{
			i--;
		}
	}
	/* search for all sub nodes */
	while (path != NULL && (current_inumber = lookup_sub_node(path, data.dirEntries)) != FAIL)
	{
		path = strtok_r(NULL, delim, &saveptr);

		/* 
		 * If path is NULL, we are about to lock our last i-node. 
		 * Therefore, we lock it in lock_mode and break the while loop
		 */
		if (path == NULL)
		{
			/* 
			 * We check the return of inode_get. If it's FAILTRY, that means 
			 * lock_mode was 't' and the trylock failed. In that case, we return
			 * FAILTRY and set the size of the lockArray.
			 */
			last_output = inode_get(current_inumber, &nType, &data, &(locks->array[i++]), lock_mode, verify_lock, lockedLock);

			if (last_output == FAILTRY)
			{
				/* The last lock failed, so we ignore the last lock in the array */
				locks->size = (i - 1);

				return FAILTRY;
			}
			else if (last_output == VERIFYSUCCESS)
			{
				i--;
			}
			break;
		}

		if (inode_get(current_inumber, &nType, &data, &(locks->array[i++]), 'r', verify_lock, lockedLock) == VERIFYSUCCESS)
		{
			i--;
		}
	}

	locks->size = i;

	return current_inumber;
}

/*
 * Lookup for a given path.
 * Input:
 *  - name: path of node
 * Returns:
 *  inumber: identifier of the i-node, if found
 *     FAIL: otherwise
 */
int lookup(char *name)
{
	int inumber;

	lockArray locks;
	locks.size = 0;

	inumber = get_inumber(name, &locks, 'r', 'n', NULL);

	unlockArray(locks);

	return inumber;
}

/*
 * Creates a new node given a path.
 * Input:
 *  - name: path of node
 *  - nodeType: type of node
 * Returns: SUCCESS or FAIL
 */
int create(char *name, type nodeType)
{

	int parent_inumber, child_inumber;
	char *parent_name, *child_name, name_copy[MAX_FILE_NAME];

	/* use for copy */
	type pType;
	union Data pdata;
	pthread_rwlock_t *plock, *clock;

	/* Used for lock management */
	lockArray locks;
	locks.size = 0;

	strcpy(name_copy, name);
	split_parent_child_from_path(name_copy, &parent_name, &child_name);

	parent_inumber = get_inumber(parent_name, &locks, 'w', 'n', NULL);

	if (parent_inumber == FAIL)
	{
		printf("failed to create %s, invalid parent dir %s\n",
			   name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	inode_get(parent_inumber, &pType, &pdata, &plock, 'n', 'n', NULL);

	if (pType != T_DIRECTORY)
	{
		printf("failed to create %s, parent %s is not a dir\n",
			   name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	if (lookup_sub_node(child_name, pdata.dirEntries) != FAIL)
	{
		printf("failed to create %s, already exists in dir %s\n",
			   child_name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	/* create node and add entry to folder that contains new node */
	child_inumber = inode_create(nodeType, &clock);

	/* 
	 * If it failed, no i-node was created, therefore no i-node is locked.
	 * In that case, we don't need to add the child lock to the lock array, 
	 * we just unlock the locks we have. 
	 */
	if (child_inumber == FAIL)
	{
		printf("failed to create %s in  %s, couldn't allocate inode\n",
			   child_name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	/* 
	 * If it didn't fail, it locked the child node in 
	 * write mode, so we add that to our lock array 
	 */
	locks.array[locks.size++] = clock;

	if (dir_add_entry(parent_inumber, child_inumber, child_name) == FAIL)
	{
		printf("could not add entry %s in dir %s\n",
			   child_name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	unlockArray(locks);

	return SUCCESS;
}

/*
 * Deletes a node given a path.
 * Input:
 *  - name: path of node
 * Returns: SUCCESS or FAIL
 */
int delete (char *name)
{

	int parent_inumber, child_inumber;
	char *parent_name, *child_name, name_copy[MAX_FILE_NAME];

	/* use for copy */
	type pType, cType;
	union Data pdata, cdata;
	pthread_rwlock_t *plock, *clock;

	/* Used for lock management */
	lockArray locks;
	locks.size = 0;

	strcpy(name_copy, name);
	split_parent_child_from_path(name_copy, &parent_name, &child_name);

	parent_inumber = get_inumber(parent_name, &locks, 'w', 'n', NULL);

	if (parent_inumber == FAIL)
	{
		printf("failed to delete %s, invalid parent dir %s\n",
			   child_name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	inode_get(parent_inumber, &pType, &pdata, &plock, 'n', 'n', NULL);

	if (pType != T_DIRECTORY)
	{
		printf("failed to delete %s, parent %s is not a dir\n",
			   child_name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	child_inumber = lookup_sub_node(child_name, pdata.dirEntries);

	if (child_inumber == FAIL)
	{
		printf("could not delete %s, does not exist in dir %s\n",
			   name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	inode_get(child_inumber, &cType, &cdata, &clock, 'w', 'n', NULL);

	/* Adds clock to the last array index, then increases its size*/
	locks.array[locks.size++] = clock;

	if (cType == T_DIRECTORY && is_dir_empty(cdata.dirEntries) == FAIL)
	{
		printf("could not delete %s: is a directory and not empty\n",
			   name);
		unlockArray(locks);
		return FAIL;
	}

	/* remove entry from folder that contained deleted node */
	if (dir_reset_entry(parent_inumber, child_inumber) == FAIL)
	{
		printf("failed to delete %s from dir %s\n",
			   child_name, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	if (inode_delete(child_inumber) == FAIL)
	{
		printf("could not delete inode number %d from dir %s\n",
			   child_inumber, parent_name);
		unlockArray(locks);
		return FAIL;
	}

	unlockArray(locks);

	return SUCCESS;
}

/*
 * Checks the first path provided in the move command to see if it's valid.
 * Inputs: 
 *  - old_parent_inumber: int containing old parent directory i-number 
 *  - old_child_name: string with old child name (can be changed by move())
 *  - old_parent_name: string with old parent directory name
 *  - child_inumber: pointer to a int to be filled with child i-number
 *  Returns: SUCESS or FAIL
 */
int old_path_verifications(int old_parent_inumber, char *old_child_name, char *old_parent_name, int *child_inumber)
{
	type old_parent_type;
	union Data old_parent_data;
	pthread_rwlock_t *old_parent_lock;

	if (old_parent_inumber == FAIL)
	{
		printf("failed to move %s, invalid parent dir %s\n",
			   old_child_name, old_parent_name);
		return FAIL;
	}

	inode_get(old_parent_inumber, &old_parent_type, &old_parent_data, &old_parent_lock, 'n', 'n', NULL);

	if (old_parent_type != T_DIRECTORY)
	{
		printf("failed to move %s, parent %s is not a dir\n",
			   old_child_name, old_parent_name);
		return FAIL;
	}

	*child_inumber = lookup_sub_node(old_child_name, old_parent_data.dirEntries);

	if (*child_inumber == FAIL)
	{
		printf("could not move %s, does not exist in dir %s\n",
			   old_child_name, old_parent_name);
		return FAIL;
	}

	return SUCCESS;
}

/*
 * Checks the second path provided in the move command to see if it's valid.
 * Inputs: 
 *  - new_parent_inumber: int containing new parent directory i-number 
 *  - new_child_name: string with new child name (can be the result of a name change)
 *  - new_parent_name: string with new parent directory name
 *  Returns: SUCESS or FAIL
 */
int new_path_verifications(int new_parent_inumber, char *new_child_name, char *new_parent_name)
{
	type new_parent_type;
	union Data new_parent_data;
	pthread_rwlock_t *new_parent_lock;

	if (new_parent_inumber == FAIL)
	{
		printf("failed to move %s(new name), invalid new parent dir %s\n",
			   new_child_name, new_parent_name);
		return FAIL;
	}

	inode_get(new_parent_inumber, &new_parent_type, &new_parent_data, &new_parent_lock, 'n', 'n', NULL);

	if (new_parent_type != T_DIRECTORY)
	{
		printf("failed to move %s(new name), new parent %s is not a dir\n",
			   new_child_name, new_parent_name);
		return FAIL;
	}

	/* Check to see if the new directory has any i-node with the same name */
	if (lookup_sub_node(new_child_name, new_parent_data.dirEntries) != FAIL)
	{
		printf("failed to move %s, already exists in dir %s\n",
			   new_child_name, new_parent_name);
		return FAIL;
	}

	return SUCCESS;
}

/*
 * Moves an i-node from one directory to another.
 * Input:
 *  - path1: Original path.
 *  - path2: New path.
 * Returns: SUCCESS or FAIL
 */
int move(char *old_path, char *new_path)
{
	/* Used for sleep management */
	long max_sleep_time = 100;

	struct timespec sleep_time, time_remaining;
	sleep_time.tv_sec = 0;

	/* Path name related variables */
	char old_path_copy[MAX_FILE_NAME], new_path_copy[MAX_FILE_NAME];
	char *old_parent_name, *old_child_name, *new_parent_name, *new_child_name;

	strcpy(old_path_copy, old_path);
	strcpy(new_path_copy, new_path);

	split_parent_child_from_path(old_path_copy, &old_parent_name, &old_child_name);
	split_parent_child_from_path(new_path_copy, &new_parent_name, &new_child_name);

	/* Used for copies */
	type old_parent_type, new_parent_type;
	union Data old_parent_data, new_parent_data;
	pthread_rwlock_t *old_parent_lock, *new_parent_lock;

	/* Lock handling */
	lockArray old_locks, new_locks;
	old_locks.size = 0;
	new_locks.size = 0;

	lockArray first_locks;
	pthread_rwlock_t *first_wr_lock;

	/* I-node handling */
	int old_parent_inumber, new_parent_inumber, child_inumber;
	int first_inumber, second_inumber;

	/* Get path depths */
	int old_path_depth = get_path_depth(old_path_copy), new_path_depth = get_path_depth(new_path_copy);

	/* If new_path is a subpath of old_path, throw an error */
	if (is_path_subpath(old_path_copy, new_path_copy) == SUCCESS)
	{
		printf("failed to move %s, new dir %s is a subpath\n", old_path_copy, new_path_copy);
		return FAIL;
	}

	while (1)
	{
		/* Each cycle, we increase the maximum sleep time by 1000 nanoseconds */
		max_sleep_time += 1000;

		/* 
	     * Lock the path with lowest depth. If they have the 
	 	 * same depth, we can lock whichever. 
	 	 */
		if (old_path_depth <= new_path_depth)
		{
			old_parent_inumber = get_inumber(old_parent_name, &old_locks, 'w', 'n', NULL);
			first_inumber = old_parent_inumber;
			first_locks = old_locks;
		}
		else
		{
			new_parent_inumber = get_inumber(new_parent_name, &new_locks, 'w', 'n', NULL);
			first_inumber = new_parent_inumber;
			first_locks = new_locks;
		}

		/* If first lock fails, just end move() */
		if (first_inumber == FAIL)
		{
			printf("failed to move %s, invalid parent dir %s\n",
				   old_child_name, old_parent_name);
			unlockArray(first_locks);
			return FAIL;
		}

		/* We need to compare any further locks to our first write lock */
		first_wr_lock = first_locks.array[first_locks.size - 1];

		if (old_path_depth <= new_path_depth)
		{
			new_parent_inumber = get_inumber(new_parent_name, &new_locks, 't', 'v', first_wr_lock);
			second_inumber = new_parent_inumber;
		}
		else
		{
			old_parent_inumber = get_inumber(old_parent_name, &old_locks, 't', 'v', first_wr_lock);
			second_inumber = old_parent_inumber;
		}

		/* If second inumber fails, just end move() */
		if (second_inumber == FAIL)
		{
			printf("failed to move %s, invalid new parent dir %s\n",
				   old_child_name, new_parent_name);
			unlockArray(old_locks);
			unlockArray(new_locks);
			return FAIL;
		}

		/* If second lock fails in the try_wrlock, sleep and loop again */
		if (second_inumber == FAILTRY)
		{
			/* Unlock and reset arrays */
			unlockArray(old_locks);
			unlockArray(new_locks);
			old_locks.size = 0;
			new_locks.size = 0;

			/* Figure out how much longer we should sleep */
			sleep_time.tv_nsec = (random() % max_sleep_time);

			/* Sleep, then loop again */
			nanosleep(&sleep_time, &time_remaining);
			continue;
		}

		/* After we get both locks, we get info about each parent i-node */
		inode_get(old_parent_inumber, &old_parent_type, &old_parent_data, &old_parent_lock, 'n', 'n', NULL);
		inode_get(new_parent_inumber, &new_parent_type, &new_parent_data, &new_parent_lock, 'n', 'n', NULL);

		if (old_parent_type != T_DIRECTORY)
		{
			printf("failed to move %s, parent %s is not a dir\n",
				   old_child_name, old_parent_name);
			unlockArray(old_locks);
			unlockArray(new_locks);
			return FAIL;
		}

		child_inumber = lookup_sub_node(old_child_name, old_parent_data.dirEntries);

		if (child_inumber == FAIL)
		{
			printf("could not move %s, does not exist in dir %s\n",
				   old_child_name, old_parent_name);
			unlockArray(old_locks);
			unlockArray(new_locks);
			return FAIL;
		}

		inode_get(new_parent_inumber, &new_parent_type, &new_parent_data, &new_parent_lock, 'n', 'n', NULL);

		if (old_parent_type != T_DIRECTORY)
		{
			printf("failed to move %s, new parent %s is not a dir\n",
				   old_child_name, new_parent_name);
			unlockArray(old_locks);
			unlockArray(new_locks);
			return FAIL;
		}

		/* Check to see if the new directory has any i-node with the same name */
		if (lookup_sub_node(new_child_name, new_parent_data.dirEntries) != FAIL)
		{
			printf("failed to move %s, already exists in dir %s\n",
				   new_child_name, new_parent_name);
			unlockArray(old_locks);
			unlockArray(new_locks);
			return FAIL;
		}

		/* remove entry from old directory */
		if (dir_reset_entry(old_parent_inumber, child_inumber) == FAIL)
		{
			printf("failed to move %s from dir %s\n",
				   old_child_name, old_parent_name);
			unlockArray(old_locks);
			unlockArray(new_locks);
			return FAIL;
		}

		/* Add entry to new directory */
		if (dir_add_entry(new_parent_inumber, child_inumber, new_child_name) == FAIL)
		{
			printf("could not move entry %s (new name) to dir %s\n",
				   new_child_name, new_parent_name);
			unlockArray(old_locks);
			unlockArray(new_locks);
			return FAIL;
		}

		unlockArray(old_locks);
		unlockArray(new_locks);

		/* End the loop */
		break;
	}

	return SUCCESS;
}

/*
 * Prints tecnicofs to a given file.
 * Input:
 *  - path: string containing a file path.
 * Returns: SUCCESS or FAIL
 */
int print_tecnicofs_tree(char *path)
{
	FILE *output_file;
	if ((output_file = fopen(path, "w")) == NULL)
	{
		fprintf(stderr, "Error opening output file.\n");
		return FAIL;
	}

	/* If you lock root directory in write mode, no other operation CAN happen */
	lock_root();

	inode_print_tree(output_file, FS_ROOT, "");

	/* After you print, unlock the root directory */
	unlock_root();

	if (fclose(output_file) != 0)
	{
		fprintf(stderr, "Error closing file.\n");
		return FAIL;
	}

	return SUCCESS;
}