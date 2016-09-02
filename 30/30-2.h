#ifndef TREE_H
#define TREE_H
#include <pthread.h>
typedef enum {FALSE, TRUE} bool;

struct{
    char *key;
    void *value;
} data_type;

struct {
  data_type data;
  tree_type *left;
  tree_type *right;
  tree_type *parent;
  pthread_mutex_t lock;
}tree_type;

typedef int (*compare)(char *one, char*two);

bool initialize(tree_type *);
bool add(tree_type *,char *, void *,compare);
bool delete(tree_type *,char *);
bool lookup(tree_type *,char *);
