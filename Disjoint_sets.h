
#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#ifndef nullptr
#define nullptr 0
#endif


#include <limits>
#include "Exception.h"

using namespace std;

struct ll_entry; // represents each element of the linked list
struct set_info; // keeps track of the information (pointers to head and tail and the size of the set) of each set
//could we delete the above two lines?


struct ll_entry{ 
	int content;
	set_info* ptr_to_info; // ptr to the info entry of the corresponding set
	ll_entry* next;
	ll_entry(int value){
		ptr_to_info = nullptr;
		next = nullptr;
		content = value;
	}
};

struct set_info { 
	ll_entry* head;
	ll_entry* tail;
	int size;
	set_info(ll_entry* insert_head, ll_entry* insert_tail){
		head = insert_head;
		tail = insert_tail;
		size = 1;
	}
};

class Disjoint_set {

private:
	ll_entry** nodes;
	set_info** sets;
	int set_counter;
	int initial_num_sets;
public:
	Disjoint_set(int);
	~Disjoint_set();
	int find_set(int) const;
	int num_sets() const;
	void union_sets(int, int);
	void create_set(int);
};

Disjoint_set::Disjoint_set(int n) : nodes(new ll_entry*[n]), 
sets (new set_info*[n]), 
set_counter(n),
initial_num_sets(n) {
	// initialize the sets	
}

Disjoint_set::~Disjoint_set() {
	// deallocate memory
	for (int i = 0; i < initial_num_sets; i++){
		if (nodes[i] != nullptr)
			delete nodes[i];
		if (sets[i] != nullptr)
			delete sets[i];
	}
	
	delete[]nodes;
	delete[]sets;
	
	set_counter = 0;
	initial_num_sets = 0;
}
int Disjoint_set::find_set(int item) const{
	return nodes[item] -> ptr_to_info -> head -> content;
}

int Disjoint_set::num_sets() const {
	return set_counter;
}

void Disjoint_set::union_sets(int node_index1, int node_index2) {
	
	if (node_index1 == node_index2)
		return;
	
	set_info* si1 = nodes[node_index1]->ptr_to_info;
	set_info* si2 = nodes[node_index2]->ptr_to_info;

	// ni1: the index of the larger set, ni2: the index of the smaller index
	int ni1 = si1->size >= si2->size ? node_index1 : node_index2; 
	int ni2 = si1->size < si2->size ? node_index1 : node_index2;
	
	si1 = nodes[ni1]->ptr_to_info; //si1 = larger set
	si2 = nodes[ni2]->ptr_to_info; //si2 = smaller set
	
	ll_entry *s2_traverse = si2 -> head;
	
	while(s2_traverse != nullptr){
		
		s2_traverse -> ptr_to_info = si1;
		s2_traverse = s2_traverse -> next;		
	}

	// do we need to modify anything else?
	si1 -> tail -> next = si2 -> head;
	si1 -> tail = si2 -> tail;
	si1 -> size = si1 -> size + si2 -> size;

	// delete the set_info entry that no longer exists
	si2 -> head = nullptr;
	si2 -> tail = nullptr;
	si2 = nullptr;
	//delete si2 -> head;
	//delete si2 -> tail;
	//delete si2;
	
	set_counter--;
	return;
	
}

void Disjoint_set::create_set (int node_index){
	ll_entry* node = new ll_entry(node_index);
	nodes[node_index] = node;
	set_info* si = new set_info(node, node);
	nodes[node_index] -> ptr_to_info = si;
	sets[node_index] = si;
	
}

#endif
