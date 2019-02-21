/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Jeremiah Kramer
 * Email: kramerje@oregonstate.edu
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

struct node {
	int pid; //priority id
	void* value; //value of node
};

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
	struct dynarray* da;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* pq = malloc(sizeof(struct pq));
	assert(pq);
	pq->da = dynarray_create();
	return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	assert(pq);
	dynarray_free(pq->da);
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	//if size of array is 0, then it's empty
	if(dynarray_size(pq->da) == 0) 
		return 1;
	else
		return 0;
}


/*
 * This is a helper function for pq_insert
 * percolate up to re-heapify when inserting
 *
 * Params: 
 *	da - dynarray we are using to re-heapify
 *	new_idx - the node's index that was just inserted
 *	new_node - node that was just inserted into dynarray
 */
void percolate_up(struct dynarray* da, int new_idx, struct node* new_node){
	//while the idx we are percolating up with is not at the root (greater than 0)
	while(new_idx > 0){
		//calculate parent index
		int par_idx = ((new_idx - 1) / 2);
		//get parent node
		struct node* parent = dynarray_get(da, par_idx);
		//if the priority value of the parent is greater than 
		//priority value of the newly inserted node
		if(parent->pid > new_node->pid){
			//swap
			dynarray_set(da, par_idx, new_node);
			dynarray_set(da, new_idx, parent);
			new_idx = par_idx;
		}
		//if it isn't
		else{
			//break out of loop
			return;
		}
	}
}

/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	//allocate memory on heap to store new node
	struct node* n = malloc(sizeof(struct node));
	//store values in new node that are passed in as params
	n->pid = priority;
	n->value = value;
	//insert new node at end of array
	dynarray_insert(pq->da, -1, n);
	//get new node idx (pass to percolate up)
	int new_idx = (dynarray_size(pq->da) - 1);
	//if there's more than one node in the array
	if(dynarray_size(pq->da) != 0){
		//percolate up
		percolate_up(pq->da, new_idx, n);
	}
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	return ((struct node*)dynarray_get(pq->da, 0))->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	return ((struct node*)dynarray_get(pq->da, 0))->pid;
}

/*
 * This is a helper function for pq_remove_first
 * percolate down to re-heapify when inserting
 *
 * Params: 
 *	da - dynarray we are using to re-heapify
 *	idx - index we are percolating down from
 */
void percolate_down(struct dynarray* da, int idx){
	int min_child_idx, left_child_idx, right_child_idx, size_idx;
	//calculate left and right child indexes
	left_child_idx = ((2 * idx) + 1);
	right_child_idx = ((2 * idx) + 2);
	//get last index of array for making things easier
	size_idx = dynarray_size(da) - 1;
	//get node we are percolating down from
	struct node* curr_node = dynarray_get(da, idx);
	//if the right child idx is greater than last index of array
	if(right_child_idx > size_idx){
		//if the left child idx is greater than last index of array
		if(left_child_idx > size_idx){ 
			//stop and break out of loop, this node doesn't have any children (leaf node)
			return;
		}
		//if there doesn't exist a right child
		else{
			//set minimum child index to left child index
			min_child_idx = left_child_idx;
		}
	}
	//if there are two children
	else{
		//get left and right child nodes
		struct node* left_child = dynarray_get(da, left_child_idx);
		struct node* right_child = dynarray_get(da, right_child_idx);
		//if the left child priority value is less than right child priority value
		if(left_child->pid <= right_child->pid){
			//set minimum child index to left child index
			min_child_idx = left_child_idx;
		}
		//if the right child priority value is less than or equal to left child priority value
		else{
			//set minimum child index to right child index
			min_child_idx = right_child_idx;
		}
	}
	//get minimum child node with this new min child idx
	struct node* min_child = dynarray_get(da, min_child_idx);
	//if the priority value of the node we are percolating down with
	//is greater than minimum child node's priority value
	if(curr_node->pid > min_child->pid){
		//swap
		dynarray_set(da, min_child_idx, curr_node);
		dynarray_set(da, idx, min_child);
		//get new idx of node we are percolating down
		idx = min_child_idx;
		//recursively call percolate down with new idx 
		percolate_down(da, idx);
	}
	
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	//save first value of array in order to return it later
	void* first_val = pq_first(pq);
	//get the first node of array in order to free it from the array
	struct node* first_node = dynarray_get(pq->da, 0);
	//free the first node
	free(first_node);
	//if the there's more than one node in the array
	if(dynarray_size(pq->da) > 0){
		//get the node from the end of the array
		struct node* new_first = dynarray_get(pq->da, -1);
		//insert this node from the end at the front
		dynarray_set(pq->da, 0, new_first);
		//percolate down
		percolate_down(pq->da, 0);
		//remove the last node in array (basically just decrements size)
		dynarray_remove(pq->da, -1);
	}
	//return value of first node we saved at the beginning of func
 	return first_val;
}
