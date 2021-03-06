#ifndef FS_H
#define FS_H
#include "state.h"

void init_fs();
void destroy_fs();
int is_dir_empty(DirEntry *dirEntries);
int create(char *name, type nodeType);
int delete (char *name);
int lookup(char *name);
int move(char *path1, char *path2);
int print_tecnicofs_tree(char *path);

#endif /* FS_H */
