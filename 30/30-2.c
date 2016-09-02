#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "30-2.h"

static pthread_mutex_t malloc_lock = PTHREAD_MUTEX_INITIALIZER;

bool initialize(tree_type *tree){
  memset(tree,0,sizeof(tree));
  tree->parent = tree;
  if(pthread_mutex_init(&tree->lock,NULL) == -1){
    return FALSE;
  }
  return TRUE;
}

bool add(tree_type *tree,char * key, void *value, compare fct){
  if(tree == NULL || key == NULL || value == NULL || fct == NULL){
    return FALSE;
  }
  tree_type *ptr = tree;
  while(1){
    pthread_mutex_lock(&ptr->mutex);
    int cmp;
    if((cmp=fct(ptr->key,key)) > 0){
      tree_type *prev = ptr;
      ptr = ptr->right;
      if(ptr == NULL){
        pthread_mutex_lock(&malloc_lock);
        prev->right = malloc(sizeof(tree_type));
        prev->right->right = NULL;
        prev->right->left  = NULL;
        prev->right->parent = prev;
        if(pthread_mutex_init(&prev->right->lock) == -1)
          return FALSE;
        prev->right->data->key = key;
        prev->right->data->value = value;
        pthread_mutex_unlock(&malloc_lock);
        pthread_mutex_unlock(&ptr->mutex);
        return TRUE;
      }

    }
    else if(cmp < 0){
      tree_type *prev = ptr;
      ptr = ptr->left;
      if(ptr == NULL){
        pthread_mutex_lock(&malloc_lock);
        prev->left = malloc(sizeof(tree_type));
        prev->left->right = NULL;
        prev->left->left  = NULL;
        prev->left->parent = prev;
        if(pthread_mutex_init(&prev->left->lock) == -1)
          return FALSE;
        prev->left->data->key = key;
        prev->left->data->value = value;
        pthread_mutex_unlock(&malloc_lock);
        pthread_mutex_unlock(&ptr->mutex);
        return TRUE;
      }
    }
    else{
      ptr->value = value;
      pthread_mutex_unlock(&ptr->lock);
      return TRUE;
    }
    pthread_mutex_unlock(&ptr->lock);
  }

}

bool lookup(tree_type *tree,char *key,compare fct){
  if(tree == NULL || key == NULL || fct == NULL){
    errno = EINVAL;
    return FALSE;
  }

  tree_type *ptr = tree;
  while(ptr!=NULL){
    pthread_mutex_lock(&ptr->lock);
    int cmp;
    if((cmp = fct(ptr->key,key)) > 0)
      ptr = ptr->right;
    else if(cmp < 0)
      ptr = ptr->left;
    else{
      pthread_mutex_unlock(&ptr->lock);
      return TRUE;
    }

    pthread_mutex_unlock(&ptr->lock);

  }
  return FALSE;

}

bool delete(tree_type *tree, char *key, compare fct){
  if(tree == NULL || key == NULL || fct == NULL){
    errno = EINVAL;
    return FALSE;
  }
  tree_type *ptr = tree;
  tree_type *prev = NULL;
  while(ptr!=NULL){
    int cmp;
    if((cmp = fct(ptr->key,key)) > 0)
      ptr = ptr->right;
    else if(cmp<0)
      ptr = ptr->right;
    else{
      if(prev->right != NULL &&fct(prev->right->key,ptr->key) == 0)
        prev->right = NULL;
      else
        prev->left = NULL;
      tree_type *start = (prev->left == NULL) : prev->right : prev->left;
      if(start == NULL)
        break;
      while()
    }
  }

}
