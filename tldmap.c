#include <stdio.h>
#include <stdlib.h>
#include "date.h"
#include "iterator.h"
#include "tldmap.h"
#include "string.h"
typedef struct tldnode{
	long value;
	char *tld;
	TLDNode *left;
	TLDNode *right;
}TLDNode;

typedef struct bst{
	unsigned long size;
	unsigned long count;
	TLDNode *root;
}BST;


void inorderprint(TLDNode *root){
	if(root == NULL){
	    return;
	}
	    inorderprint(root->left);
	    printf("tld: %s value: %ld \n", root->tld, root->value);
	    inorderprint(root->right);
}
/*
 * destroy() destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the map is returned to the heap
 */
void inorderfree(TLDNode *root){

	if(root == NULL){
	    return;
	}
	    inorderfree(root->left);
	    inorderfree(root->right);
	    free(root->tld);
	    free(root);
}

static void destroy(const TLDMap *tld){
	BST *tree = (BST *)tld->self;
	inorderfree(tree->root);
	free(tree);
	free((void *)tld);
}


/*
 * insert() inserts the key-value pair (theTLD, v) into the map;
 * returns 1 if the pair was added, returns 0 if there already exists en
 * entry corresponding to TLD
 */
TLDNode* createNode(char *theTLD, long v){
	TLDNode *n = (TLDNode *)malloc(sizeof(TLDNode));
	
	char *s = strdup(theTLD);
	int i = strlen(s);
	if(n != NULL){
	    n->value = v;
	    n->tld = s;
	    n->left = NULL;
	    n->right = NULL;
	    return n;
	}else{
	    free(n);
	    return NULL;
	}
}

static int insert(const TLDMap *tld, char *theTLD, long v){

	BST *tree = (BST *)tld->self;
	tree->size++;
	TLDNode **update;
	TLDNode *temp;
	update = &(tree->root);
	temp = tree->root;
	while(temp != NULL){
	    char *l, *r;
	    int c;
	    l = temp->tld;
	    r = theTLD;
	    c = strcmp(r,l);
		//traverse down the left
	    if(c < 0){
	        update = &(temp->left);
	        temp = temp->left;
	    }
		//traverse down the right
	    else if(c > 0){ 
	        update = &(temp->right);
	        temp = temp->right;
	    }
	    else if(c ==0){
		return 0;
	    }
	}
	*update = createNode(theTLD, v);
	return 1;


}

/*
 * reassign() replaces the current value corresponding to theTLD in the
 * map with v.
 * returns 1 if the value was replaced, returns 0 if no such key exists
 */

static int reassign(const TLDMap *tld, char *theTLD, long v){
	BST *tree = (BST *)tld->self;
	TLDNode *temp;
	temp = tree->root;
	while(temp != NULL){
	    char *l, *r;
	    int c;

	    if(strcmp(temp->tld, theTLD) == 0){
		temp->value = v;
		return 1;
	    }
	    if(temp->left == NULL && temp->right == NULL){
		return 0;
	    }
	    l = temp->tld;
	    r = theTLD;
	    c = strcmp(r , l);
		//traverse down the left
	    if(c < 0){
	        temp = temp->left;
	    }
		//traverse down the right
	    else if(c > 0){ 
	        temp = temp->right;
	    }
	}
	return 0;



}


/*
 * lookup() returns the current value associated with theTLD in *v
 * returns 1 if the lookup was successful, returns 0 if no such key exists
 */
static int lookup(const TLDMap *tld, char *theTLD, long *v){	
	BST *tree = (BST *)tld->self;
	TLDNode *temp;
	temp = tree->root;
	while(temp != NULL){
	    char *l, *r;
	    int c;

	    if(strcmp(temp->tld, theTLD) == 0){
		*v = temp->value;
		return 1;
	    }
	    if(temp->left == NULL && temp->right == NULL){
		return 0;
	    }
	    l = temp->tld;
	    r = theTLD;
	    c = strcmp(r , l);
		//traverse down the left
	    if(c < 0){
	        temp = temp->left;
	    }
		//traverse down the right
	    else if(c > 0){ 
	        temp = temp->right;
	    }
	}
	return 0;
}

void inorder(TLDNode *root, TLDNode **arr, BST *counter){
	if(root == NULL){
	    return;
	}
	    inorder(root->left, arr, counter);
	    arr[counter->count] = root;
	    counter->count++;
	    inorder(root->right, arr, counter);


}

/*
 * itCreate() creates an iterator over the map
 * returns the iterator if successful, NULL if unsuccessful
 */
static const Iterator *itCreate(const TLDMap *tld){
	BST *tree = (BST *)tld->self;
	const Iterator *it = NULL;
	TLDNode **tmp = (TLDNode **)malloc(sizeof(TLDNode*) * tree->size);
	inorder(tree->root, tmp, tree);
	it = Iterator_create(tree->size, (void **)tmp);	
	return it;
}

static TLDMap template = {
	NULL, destroy, insert, reassign, lookup, itCreate 
};
/*
 * TLDNode_tldname returns the tld associated with the TLDNode
 */
char *TLDNode_tldname(TLDNode *node){
	return node->tld;
}

/*
 * TLDNode_count returns the value currently associated with that TLD
 */
long TLDNode_count(TLDNode *node){
	return node->value;
	
}

const TLDMap *TLDMap_create(void){
	TLDMap *tld = (TLDMap *)malloc(sizeof(TLDMap));
	BST *bst = (BST *)malloc(sizeof(BST));    
	bst->root = NULL;
	bst->size = 0L;
	bst->count = 0L;
	
	if(bst != NULL && tld != NULL){
	    *tld = template;
	    tld->self = bst;
	    return tld;
	}
	else{
		free(tld);
		free(bst);
		return NULL;
	}
}

