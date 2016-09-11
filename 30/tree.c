

#include "tree.h"







bool intialize(struct tree_type *root){
	if(root ==NULL){
		errno = EINVAL;
		return FALSE;
	}
	root->tr_data.key = NULL;
	root->tr_left = NULL;
	root->tr_right = NULL;
	if(pthread_mutex_init(&root->lock,NULL)!=0)
		return FALSE;
	return TRUE;
}

/**
 * add child to parent
 * @parent: ptr to parent node
 * @key: key to add to new node
 * @value: value to add to new node
 * which: enum specificed as RIGHT chld or LEFT chld
 * returns : status
 * **/
static bool add_node(struct tree_type *parent,char *key, char *value,which way){
	((way==RIGHT) ? node->right : node->left) = (struct tree_type *)malloc(sizeof(struct tree_type));
	if( (way==RIGHT)?node->right :node->left == NULL)
		return FALSE;
	((way==RIGHT) ?node->right.key: node->left.key) = key;
	((way==RIGHT) ?node->right.value: node->left.value) = value;	
	return TRUE;
}

bool add(struct tree_type *root,char *key, void *value){
	if(root == NULL || key == NULL || value == NULL){
		errno = EINVAL;
		return FALSE;
	}
	
	if(pthread_mutex_lock(&root->lock)!=0)
		return FALSE;
	struct tree_type *node = root;
	if(node->tr_data.key == NULL){ //if new tree with no data
		node->tr_data.key = key;
		node->tr_data.value = value;
		if(pthread_mutex_unlock(&node->lock)!=0)
			return FALSE;
		return TRUE;
	}
	else if(node->tr_data.key == key){ //if key is root
		pthread_mutex_unlock(&node->lock);
		errno = EEXIST;
		return FALSE;
	}
	int cmp_res =0;
	while(TRUE){
		if((cmp_res=strcmp(node->tr_data.key,key))>0){
		       struct tree_type *tmp = node->left;				
		       if(tmp == NULL){
			       if(!add_node(node,key,value,LEFT))
				       return FALSE;
			       if(pthread_mutex_unlock(&node->lock)!=0)
				       return FALSE;
			       return TRUE;
		       }
		       else{
			    if(pthread_mutex_unlock(&node->lock)!=0)
				    return FALSE;
			    node = tmp;
			    if(pthread_mutex_lock(&node->lock)!=)
				    return FALSE;

		       }

			continue;
		}
		else if(cmp_res<0){
		      struct tree_type *tmp = node->right;				
		       if(tmp == NULL){
			       if(!add_node(node,key,value,RIGHT))
				       return FALSE;
			       if(pthread_mutex_unlock(&node->lock)!=0)
				       return FALSE;
			       return TRUE;
		       }
		       else{
			    if(pthread_mutex_unlock(&node->lock)!=0)
				    return FALSE;
			    node = tmp;
			    if(pthread_mutex_lock(&node->lock)!=)
				    return FALSE;

		       }
		}
		else{
			errno = EEXIST;
			return FALSE;
		}

	}



	



}


