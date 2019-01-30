
/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  vskottur @uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  Fall 2017
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    n2adil
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    n2adil - implementation, testing and debugging
 *****************************************/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <limits>
#include "Exception.h"
#include "Disjoint_sets.h"

using namespace std;

struct Edge{
	double weight;
	int i_vertex;
	int j_vertex;
	Edge* prevEdge;
	Edge* nextEdge;
	Edge(double w, int vertex_i, int vertex_j){
		weight = w;
		i_vertex = vertex_i;
		j_vertex = vertex_j;
		prevEdge = nullptr;
		nextEdge = nullptr;
	}
	
};

struct EdgeList{
	int list_size;
	Edge* head;
	Edge* tail;
	EdgeList(){
		head = nullptr;
		tail = nullptr;
		list_size = 0;
	}
};

struct AdjacentVertex{
	int vertex_value;
	AdjacentVertex* previous;
	AdjacentVertex* next;
	Edge* edge;
	
	AdjacentVertex(int value, Edge* edge_pointer){
		vertex_value = value;
		previous = nullptr;
		next = nullptr;
		edge = edge_pointer;
	}
};

struct AdjacencyList{
	int list_size;
	AdjacentVertex* head;
	AdjacentVertex* tail;
	AdjacencyList(){
		head = nullptr;
		tail = nullptr;
		list_size = 0;
	}
};


class Weighted_graph {
	private:
		static const double INF;
		//double** graph;
		int num_nodes;
		//int num_edges;
		
		AdjacencyList** adjLists;
		EdgeList* edgeLists;
		

		// Do not implement these functions!
		// By making these private and not implementing them, any attempt
		// to make copies or assignments will result in errors
		Weighted_graph( Weighted_graph const & );
		Weighted_graph &operator=( Weighted_graph );

		// your choices
		AdjacentVertex* findAdjacent(AdjacencyList*, int);
		void insertAdjacentEdges(Edge*&);
		void removeAdjacentEdges(Edge*&);
		void insertAdjacentVertex(AdjacencyList*&, int, Edge*);
		void removeAdjacentVertex(AdjacencyList*&, AdjacentVertex*&);

	public:
		Weighted_graph( int = 10 );
		~Weighted_graph();

		int degree( int ) const;
		int edge_count() const;
		std::pair<double, int> minimum_spanning_tree() const;

		bool insert_edge( int, int, double );
		bool erase_edge( int, int );
		void clear_edges();

	// Friends

	friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

Weighted_graph::Weighted_graph(int n ) {
	
	if (n < 0)
		throw illegal_argument();
	
	//graph = new double [n];
	
	num_nodes = n;
	//num_edges = 0;
	edgeLists = new EdgeList();
	adjLists = new AdjacencyList*[n];
	for(int i = 0; i < n; i++){
		adjLists[i] = new AdjacencyList();
	}
	
	
}

Weighted_graph::~Weighted_graph() {
	//delete[]graph;
	clear_edges();
	for(int i = 0; i < num_nodes; i++){
		delete adjLists[i];
	}
	delete[] adjLists;
	delete edgeLists;
	num_nodes = 0;
	//num_edges = 0;
}

int Weighted_graph::degree(int u) const {
	
	if (u < 0 || u > num_nodes - 1) {
		throw illegal_argument();
	}
	return adjLists[u] -> list_size;
	
	/* if(!(u >= 0 && u < num_nodes)){
		throw illegal_argument();
	}
	
	return adjLists[u] -> list_size; */

}

int Weighted_graph::edge_count() const {
	return edgeLists -> list_size;
}


bool Weighted_graph::erase_edge(int i, int j) {
	if(i == j)
		return true;
	
	if ((i < 0 || i > num_nodes - 1) || (j < 0 || j > num_nodes - 1))
		throw illegal_argument();
	
	/* if(!(i >= 0 && i < num_nodes) || !(j >= 0 && j < num_nodes))
		throw illegal_argument(); */
	
	AdjacencyList* adjList_i = adjLists[i];
	AdjacencyList* adjList_j = adjLists[j];	
	//Finding vertex j in the adjacent list of vertex i and vice versa
	AdjacentVertex* adjVertex_i = findAdjacent(adjList_i, j);
	AdjacentVertex* adjVertex_j = findAdjacent(adjList_j, i);
	
	if(adjVertex_i == nullptr || adjVertex_j == nullptr)
		return false;
	
	removeAdjacentEdges(adjVertex_i -> edge);
	removeAdjacentVertex(adjList_i, adjVertex_i);
	removeAdjacentVertex(adjList_j, adjVertex_j);

	return true;
}


void Weighted_graph::clear_edges() {
	
	for(int i = 0; i < num_nodes; i++){
		AdjacentVertex* adjVertex = adjLists[i] -> head;
		
		while(adjVertex != nullptr){
			if(adjVertex -> next == nullptr){
				delete adjVertex;
				adjVertex = nullptr;
			}
			
			else{
				adjVertex = adjVertex -> next;
				delete adjVertex -> previous;
			}
		}
		
		adjLists[i] -> head = nullptr;
		adjLists[i] -> tail = nullptr;
		adjLists[i] -> list_size = 0;
	}		
		
	Edge* traverseEdge = edgeLists -> head;
	while(traverseEdge != nullptr){
		if(traverseEdge -> nextEdge == nullptr){
			delete traverseEdge;
			traverseEdge = nullptr;
		}
		
		else{
			traverseEdge = traverseEdge -> nextEdge;
			delete traverseEdge -> prevEdge;
		}
	}
		
	edgeLists -> head = nullptr;
	edgeLists -> tail = nullptr;
	edgeLists -> list_size = 0;	


	return;
}


bool Weighted_graph::insert_edge( int i, int j, double d ) {
	
	if(i < 0 || i > num_nodes - 1)
		throw illegal_argument();
	
	if(j < 0 || j > num_nodes - 1)
		throw illegal_argument();
	
	if(d <= 0)
		throw illegal_argument();
	
	if(i == j)
		return false;
	
	AdjacencyList* adjList_i = adjLists[i];
	AdjacencyList* adjList_j = adjLists[j];
	//Finding vertex j in the adjacent list of vertex i and vice versa
	AdjacentVertex* adjVertex_i = findAdjacent(adjList_i, j);
	AdjacentVertex* adjVertex_j = findAdjacent(adjList_j, i);
	
		
	if(adjVertex_i != nullptr && adjVertex_j != nullptr){
		removeAdjacentEdges(adjVertex_j -> edge);
		Edge* tempEdge = new Edge(d, i, j);
		insertAdjacentEdges(tempEdge);		
		adjVertex_i -> edge = tempEdge;
		adjVertex_j -> edge = tempEdge;
	}
	
	else{
		Edge* tempEdge = new Edge(d, i, j);
		insertAdjacentEdges(tempEdge);
		insertAdjacentVertex(adjList_i, j, tempEdge);
		insertAdjacentVertex(adjList_j, i, tempEdge);
	}
	
	return true;
}


AdjacentVertex* Weighted_graph::findAdjacent(AdjacencyList* adjListFind, int valueVertex){
	AdjacentVertex* traverseVertex = adjListFind -> head;
	
	while(traverseVertex != nullptr){
		if (traverseVertex -> vertex_value == valueVertex){
			return traverseVertex;
		}
		traverseVertex = traverseVertex -> next;
	}
	return nullptr;
}


void Weighted_graph::insertAdjacentVertex(AdjacencyList*& adjListInsert, int valueVertex, Edge* edgeInsert){
	AdjacentVertex* adjVertex = new AdjacentVertex(valueVertex, edgeInsert);
	
	if(adjListInsert -> head == nullptr)
		adjListInsert -> head = adjVertex;
	
	else{
		AdjacentVertex* oldTail = adjListInsert -> tail;
		adjVertex -> previous = oldTail;
		oldTail -> next = adjVertex;	
	}
	
	adjListInsert -> tail = adjVertex;
	adjListInsert -> list_size++;
	
	return;
	
}


void Weighted_graph::insertAdjacentEdges(Edge*& edgeInsert){
	if(edgeLists -> head == nullptr){
		edgeLists -> head = edgeInsert;
		edgeLists -> list_size++;
		return;
	}
	
	Edge* traverseEdge = edgeLists -> head;
	
	if(edgeInsert -> weight <= traverseEdge -> weight){
		edgeLists -> head = edgeInsert;
		edgeInsert -> nextEdge = traverseEdge;
		traverseEdge -> prevEdge = edgeInsert;
		
		if(traverseEdge -> nextEdge == nullptr)
			edgeLists -> tail = traverseEdge;
		edgeLists -> list_size++;
		
	}
	
	else{
		while((traverseEdge -> nextEdge != nullptr) && (edgeInsert -> weight >= traverseEdge -> weight)){
			traverseEdge = traverseEdge -> nextEdge;
		}
		
		if(edgeInsert -> weight <= traverseEdge -> weight){
			traverseEdge -> prevEdge -> nextEdge = edgeInsert;
			edgeInsert -> prevEdge = traverseEdge -> prevEdge;
			traverseEdge -> prevEdge = edgeInsert;
			edgeInsert -> nextEdge = traverseEdge;
			
			edgeLists -> list_size++;
		}
		
		else{
			traverseEdge -> nextEdge = edgeInsert;
			edgeInsert -> prevEdge = traverseEdge;
			edgeLists -> tail = edgeInsert;
			
			edgeLists -> list_size++;
		}	
	}
	return;
}

void Weighted_graph::removeAdjacentVertex(AdjacencyList*& adjListRemove, AdjacentVertex*& adjVertexRemove){
	AdjacentVertex* prevVertex = adjVertexRemove -> previous;
	AdjacentVertex* nextVertex = adjVertexRemove -> next;
	
	if(prevVertex == nullptr && nextVertex != nullptr){
		nextVertex -> previous = nullptr;
		adjListRemove -> head = nextVertex;
	}
	
	else if(prevVertex != nullptr && nextVertex == nullptr){
		prevVertex -> next = nullptr;
		adjListRemove -> tail = prevVertex;
	}
	
	else{
		prevVertex -> next = nextVertex;
		nextVertex -> previous = prevVertex;
	}
	
	delete adjVertexRemove;
	adjListRemove -> list_size--;
	return;
}


void Weighted_graph::removeAdjacentEdges(Edge*& edgeRemove){
	Edge* previousEdge = edgeRemove -> prevEdge;
	Edge* nextEdge = edgeRemove -> nextEdge;
	
	if(previousEdge == nullptr && nextEdge != nullptr){
		nextEdge -> prevEdge = nullptr;
		edgeLists -> head = nextEdge;
	}
	
	else if(previousEdge != nullptr && nextEdge == nullptr){
		previousEdge -> nextEdge = nullptr;
		edgeLists -> tail = previousEdge;
	}
	
	else if(previousEdge != nullptr && nextEdge != nullptr){
		previousEdge -> nextEdge = nextEdge;
		nextEdge -> prevEdge = previousEdge;
	}
	
	delete edgeRemove;
	edgeLists -> list_size--;
	return;
	
}


// Implement using kruskal's algorithm
std::pair<double, int> Weighted_graph::minimum_spanning_tree() const {
	
	int edges_tested = 0;
	double weight_MST = 0;
	
	Disjoint_set* disjoint_set = new Disjoint_set(num_nodes);
	for(int i = 0; i < num_nodes; i++){
		disjoint_set -> create_set(i);
	}
	
	Edge* traverseEdge = edgeLists -> head;
	while(traverseEdge != nullptr && disjoint_set -> num_sets() > 1){
		if(disjoint_set -> find_set(traverseEdge -> i_vertex) != disjoint_set -> find_set(traverseEdge -> j_vertex)){
			disjoint_set -> union_sets(traverseEdge -> i_vertex, traverseEdge -> j_vertex);
			weight_MST += traverseEdge -> weight;
		}
		
		edges_tested++;
		traverseEdge = traverseEdge -> nextEdge;
	}
	
	delete disjoint_set;
	
	return std::pair<double, int>( weight_MST, edges_tested );
}

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	return out;
}


#endif
