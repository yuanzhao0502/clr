/*
 * clr_graph.h
 *
 *  Created on: Mar 3, 2014
 *      Author: mac-97-41
 */

#ifndef CLR_GRAPH_INTERFACE_H_
#define CLR_GRAPH_INTERFACE_H_
#include<vector>
#include<string>
#include<cstddef>
#include<clr_param.h>
//For asserts
#include<assert.h>
#include<metis_function.h>
#include<fstream>
#include<deque>
#include<lemon/list_graph.h>

typedef enum{
	NOT_CONN = 0
} CONN_STATUS;
//Forward declaration of subgraph
template<class T> class Subgraph;
/* This class declares all functions that must be implemented in the derived classes. */
/* A general class for the graphs in package CLR*/
template<class T> class Graph{
public:
	//This function adds an edge to the graph, given the indices of two nodes and the edge weight
	//If successful, returns true, otherwise false
	virtual bool add_edge(int idx1, int idx2, double wgt) = 0;
	//This function adds node to the graph. If successful, returns true otherwise false.
	virtual bool add_node(const T& node) = 0;
	//This function returns all edges in a two-dimensional array
	virtual int** get_edges() = 0;
	//This function returns the pointer to a node in constant mode.
	virtual const T* get_node(int idx) = 0;
	//This function get the number of nodes in the graph
	virtual int get_node_count() = 0;
	//This function get the number of edges in the graph
	virtual int get_edge_count() = 0;
	//This function returns the edge weight between two nodes given the indexes of the nodes
	virtual double get_edge_weight(int idx1, int idx2) = 0;
	//This function returns the edge weight between two given nodes
	virtual double get_edge_weight(const T& node1, const T& node2) = 0;
	//this function returns the indexes of all the connected parts of the graph, in separated arrays
	virtual Subgraph<T>** get_conn_subgraphs() = 0;
	//This functions returns if the current graph is connected, if connected, returns 1, else returns 0
	virtual bool is_conn() = 0;
	//This function checks if there is an edge between two nodes, if connected, returns 1, else returns0
	virtual int is_edge(int idx1, int idx2) = 0;
	//This function searches a given node in the graph, if exists, the index is returns, otherwise -1 is returned.
	virtual int search_node(const T& node) = 0;
	//This function removes a node given its index, a pointer to the id is returned.
	virtual T* remove_node(int idx) = 0;
	//This function removes a node given its id. If the node exists and is successfully removed then returns true, otherwise false
	virtual bool remove_node(const T& node) = 0;


	//this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	virtual int* get_neighbours(int idx) = 0;
	//this function returns an array of the indexes of all neighbouring nodes, given the node
	virtual int* get_neighbours(T node) = 0;
	//This function returns if the current graph is already a clr cluster.
	virtual bool is_cluster(Param* p) = 0;
	//This function returns a graph_t struct to run metis
	virtual graph_t* get_graph_t() = 0;

	//the virtual destructor
	virtual ~Graph(){}

};



//This class is designed for the clustering result
template<class T>
class Cluster{
	//This vector stores the indexes of different clusters
	std::vector<Subgraph<T>*> clusters;

public:
	/*
	 * default constructor
	 */
	Cluster(){}
	/*
	 * init Cluster with one Subgraph
	 */
	Cluster(Subgraph<T>* subgraphPtr){
		clusters.push_back(subgraphPtr);
	}
	//Returns the number of the clusters
	int get_cluster_count(){
		return clusters.size();
	}

	//Retuns a score of the current cluster
	//need implementation
	int get_score(){
		return 0;
	}

	//Add a subgraph into current cluster
	bool add_subgraph(Graph<T>* subgraph){
		clusters.push_back(subgraph);
		return true;
	}
	~Cluster(){}
};





#endif /* CLR_GRAPH_H_ */
