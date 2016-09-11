#ifndef TREE_H
#define TREE_H

#include <pthread.h>

typedef enum {FALSE, TRUE} bool;
typedef enum {LEFT, RIGHT} which;

struct data_type{
 	char *key;
	void *value;
}


struct tree_type{
	struct data_type tr_data;
	struct tree_type *tr_left;
	struct tree_type *tr_right;
	pthread_mutex_t lock;
}


bool initialize(struct tree_type *);
bool add(struct tree_type *,char *,void *);
bool delete(struct tree_type *,char *);
bool lookup(struct tree_type *,char *);

#endif
