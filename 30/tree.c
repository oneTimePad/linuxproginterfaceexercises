

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

/**
 * add node to tree
 * @root: root node of the tree
 * @key: key for new node
 * @value: value for new node
 **/
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

tree_type *lowest_right_child(tree_type *node){
	struct tree_type *start = node->right;
	if(start == NULL) return node;
	if(pthread_mutex_lock(&start->lock) != 0)
		return FALSE;
	while(start->left != NULL){
		struct tree_type *tmp = start;
		start = start->left;
		if(pthread_mutex_lock(&start->lock) != 0)
			return FALSE;
		if(pthread_mutex_unlock(&tmp->lock) != 0)
			return FALSE;
	}
	return start; //keep start locked

}


bool delete(struct tree_type *root, char *key, void *value){
	
	if(root == NULL || key == NULL){
		errno = EINVAL;
		return FALSE;
	}
	struct tree_type *node = root;
	struct tree_type *parent = NULL;
	int cmp_res = 0;
	while(node!= NULL){
		if(pthread_mutex_lock(&node->lock) != 0)
			return FALSE;
		if((cmp_res = strcmp(node->key,key)) < 0 ){
			if(parent!= NULL){
				if(pthread_mutex_unlock(&parent->lock) != 0)
					return FALSE;
			}
			parent = node;
			node = node->left;
		
		}
		else if(cmp_res > 0 ){
			if(parent != NULL){
				if(pthread_mutex_unlock(&parent->lock) != 0)
					return FALSE;
			}
			parent = node;
			node  = node->right;
		
		}
		else{
			if(node->left != NULL && node->right!= NULL){
				struct tree_type *replace = lowest_right_child(node);
				if(parent->left == node){
					parent->left = replace;
					

				}
			}
		}
	}





}
bool lookup(struct tree_type *root, char *key){
	if(root == NULL || key == NULL){
		errno = EINVAL;
		return FALSE;
	}

	struct tree_type *node = root;
	int cmp_res = 0;
	while(node!=NULL){
		if(pthread_mutex_lock(&node->lock) !=0)
			return FALSE;
		if((cmp_res=strcmp(node->tr_data.key,key)) > 0){
			struct tree_type *tmp = node->left;
			if(pthread_mutex_unlock(&node->lock)!=0)
				return FALSE;
			node = tmp;
			if(pthread_mutex_lock(&node->lock) !=0)
				return FALSE;

		}
		else if(cmp_res < 0 ){
			struct tree_type *tmp = node->right;
			if(pthread_mutex_unlock(&node->lock) != 0)
				return FALSE;
			node = tmp;
			if(pthread_mutex_lock(&node->lock) != 0)
				return FALSE;
		}
		else{
			if(pthread_mutex_unock(&node->lock) != 0)
				return FALSE;
			return TRUE;
		}
	}

	return FALSE
}


