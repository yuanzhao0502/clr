/*
 * clr_subgraph.h
 *
 *  Created on: Mar 12, 2014
 *      Author: mac-97-41
 */

#ifndef CLR_SUBGRAPH_H_
#define CLR_SUBGRAPH_H_
#include<clr_graph_interface.h>



//This class gives the interface of the most general subgraph

template<class T>
class Subgraph: public Graph<T>{
public:
	//Constructor
	//This function returns a pointer to the super graph
	virtual Graph<T>* get_supergraph() = 0;
	//This function returns if an index is in the subgraph
	virtual bool in_subgraph(int index) = 0 ;
	//virtual destructor
	virtual ~Subgraph(){};
};


//This class gives some declarations of the functions in a directed subgraph
template<class T>
//class DirectedSubgraph: public Subgraph<T>{
class DirectedSubgraph: public Graph<T>{
public:
	//This function gets a set of predecessors of a given index of a node. The function returns the indexes
	//of the predecessors
	virtual int* get_predecessors(int nodeIdx) = 0;
	//This functions gets a set of predecessors of a given node. The function returns the indexes of the predecessors
	virtual int* get_predecessors(const T& node) = 0;
	//This function gets a set of successors of a given index of a node. The function returns the indexes
	//of the successors
	virtual int* get_successors(int idxNode) = 0;
	//This function gets a set of successors of a given a node. The function returns the indexes of the successors
	virtual int* get_successors(const T& node) = 0;
	//virtual destructor
	virtual ~DirectedSubgraph(){};
};

/*
//This class gives some declarations of the function in a undirected subgraph
template<class T>
class UndirectedSubgraph: Subgraph<T>{
public:
	//this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	virtual int* get_neighbours(int idxNode) = 0;
	//this function returns an array of the indexes of all neighbouring nodes, given the node
	virtual int* get_neighbours(T node) = 0;
	//virtual destructor
	virtual ~UndirectedSubgraph(){}
	//this function returns
};

*/

//This class gives the definition of UndirectedCompSubgraph, implementing all virtual functions
//Forward declaration
template<class T> class UndirectedCompGraph;
template<class T>
//class UndirectedCompSubgraph: public UndirectedSubgraph<T>{
class UndirectedCompSubgraph:public Subgraph<T>{
	std::vector<int> subgraphNodes;
	UndirectedCompGraph<T>* supergraph;

public:

	//constructor
	/**********************************************************
	 * This constructor creates a UndirectedCompSubGraph object with a node array and a pointer to the super graph
	 **********************************************************/
	UndirectedCompSubgraph(int* nodes, UndirectedCompGraph<T>* supergraph){
		this->subgraphNodes.assign(nodes,nodes+sizeof(nodes)/sizeof(int));
		this->supergraph = supergraph;
	}


	//Implement the virtual functions derived from Graph<T>
	/**********************************************************
	*This function adds node to the graph.
	*If the node is contained in the super graph, then we just add the node index into subgraphNodes.
	*If the node is not contained in the super graph, then we first add the node into supergraph and then put it into subgraphNodes.
	***********************************************************/
	bool add_node(const T& node){
		//Check if the node is in the super graph
		//If not
		int idxInSupergraph = supergraph->search_node(node);
		if(idxInSupergraph!= -1){
			supergraph->add_node(node);
			subgraphNodes.push_back(supergraph->get_node_count()-1);
		}
		//If the nodes is already in the supergraph
		else{
			subgraphNodes.push_back(idxInSupergraph);
		}
		subgraphNodes.push_back(supergraph->add_node(node));
		return true;
	}

	/**********************************************************
	 * This function adds an edge into the subgraph. However, the two
	 * incident nodes must be in the subgraph
	 **********************************************************/
	bool add_edge(int idx1, int idx2, double wgt){
		if(in_subgraph(idx1) && in_subgraph(idx2)){
			supergraph->add_edge(idx1, idx2,wgt);
			return true;
		}
		else return false;

	}


	/*********************************************************
	 * This function returns all the connected subgraphs in the current subgraph
	 *********************************************************/
	Subgraph<T>** get_conn_subgraphs(){
		//Implement later
		return 0;
	}

	/*********************************************************
	 * This function returns all edges in the subgraph, in the form of a two dimensional array.
	 * Each element of the array is a two-element subarray storing the two ends of an edge.
	 *********************************************************/
	int** get_edges(){
		//Implement later
		return 0;
	}

	/************************************************************************
	 * This functions returns a pointer to the node in the subgraph, given the index of the node.
	 ************************************************************************/
	const T* get_node(int index){
		//Check if the index is in the subgraph
		if(!in_subgraph(index))
			return 0;// Here, it's better we throw out an exception
		else return supergraph->get_node(index);
	}


	//This function get the number of nodes in the graph
	int get_node_count(){
		return subgraphNodes.size();
	}

	//This function get the number of edges in the graph
	int get_edge_count(){
		//not necessary
		return 0;
	}

	//This function returns the edge weight between two nodes given the indexes of the nodes
	double get_edge_weight(int nodeIdx1, int nodeIdx2){
		return supergraph->get_edge_weight(nodeIdx1, nodeIdx2);
	}

	//This function returns the edge weight between two given nodes
	double get_edge_weight(const T& node1, const T& node2){
		return supergraph->get_edge_weight(node1,node2);
	}

	//Implementing the virtual functions derived from UndirectedGraphs
	/*********************************************************************************************
	 *this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	 *********************************************************************************************/
	int* get_neighbours(int nodeIdx){
		std::vector<int> superGraphNeighbours;
		int* superGraphNeighboursArr = supergraph->get_neighbours(nodeIdx);
		superGraphNeighbours.assign(superGraphNeighboursArr,
				superGraphNeighboursArr+ sizeof(superGraphNeighboursArr)/sizeof(int));
		std::vector<int> neighbours;
		for(int i=0;i<superGraphNeighbours.size();i++)
			if(find(subgraphNodes.begin(), subgraphNodes.end(),superGraphNeighbours[i])!= subgraphNodes.end())
				neighbours.push_back(superGraphNeighbours[i]);
		return &neighbours[0];
	}


	/*********************************************************************************************
	 * This function returns an array of the indexes of all neighbouring nodes, given the node
	 **********************************************************************************************/
	int* get_neighbours(T node){
		return NULL;

	}


	/*********************************************************************************************
	 * This functions returns a graph_t object based on the current subgraph
	 *********************************************************************************************/
	graph_t* get_graph_t(){
		std::vector<int> sub_adjncy;
		std::vector<int> sub_adjwgt;
		std::vector<int> sub_xadj;
		//get all the edge element in adjncy related to the subgraph and push them to sub_adjncy
		for(int i=0;i<subgraphNodes.size();i++){
			//Move the corresponding element to sub_adjncy
			copy(supergraph->adjncy.begin()+supergraph->xadj[subgraphNodes[i]],
					supergraph->adjncy.begin()+supergraph->xadj[subgraphNodes[i]+1], sub_adjncy.end()-1);
			copy(supergraph->adjwgt.begin()+supergraph->xadj[subgraphNodes[i]],
					supergraph->adjwgt.begin()+supergraph->xadj[subgraphNodes[i]+1], sub_adjwgt.end()-1);
			//Write to sub_xadj
			/*If the size of sub_xadj is equal to 0, then we know the first node starts from index 0 in the adjncy*/
			if(sub_xadj.size() == 0)
				sub_xadj.push_back(0);
			/*Else, we push back the element "number of neighbours + start index"*/
			else
				sub_xadj.push_back(sub_xadj[sub_xadj.size()-1] +
						supergraph->xadj[subgraphNodes[i]+1]-supergraph->xadj[subgraphNodes[i]]);

		}
		//construct a graph_t object
		graph_t* graph = new graph_t();
		//assign the value xadj, adjncy and adjwgt
		graph->adjncy = &sub_adjncy[0];
		graph->adjwgt = &sub_adjwgt[0];
		graph->xadj = &sub_xadj[0];
		graph->vsize = NULL;
		graph->tvwgt = NULL;

		//The assignment of all following variables need checking
		graph->free_adjncy = -1;
		graph->free_adjwgt = -1;
		graph->free_vsize = -1;
		graph->free_vwgt = -1;
		graph->free_xadj = -1;

		graph->ckrinfo = NULL;
		graph->mincut = -1;
		graph->minvol = -1;
		graph->where = NULL;
		graph->pwgts = NULL;
		graph->nbnd = -1;
		graph->bndind = NULL;
		graph->bndptr = NULL;
		graph->id = NULL;
		graph->ed = NULL;
		graph->vkrinfo = NULL;
		graph->nrinfo = NULL;
		graph->coarser = NULL;
		graph->finer = NULL;

		return graph;
	}


	/**********************************************************
	 *
	 **********************************************************/
	Graph<T>* get_supergraph(){
		return supergraph;
	}


	/**********************************************************
	*This function checks if the given index is in the subgraph
	**********************************************************/
	bool in_subgraph(int index){
		if(index <0 || find(subgraphNodes.begin(), subgraphNodes.end(), index)
				== subgraphNodes.end())
			return false;
		else return true;
	}

	//This functions returns if the current graph is connected, if connected, returns 1, else returns 0
	bool is_conn(){
		int* breadthSearchNodes = breadth_first_search(subgraphNodes[0],this);
		if(sizeof(breadthSearchNodes)/sizeof(int) != subgraphNodes.size())
			return false;
		else return true;
	}

	//This function checks if there is an edge between two nodes, if connected, returns 1, else returns0
	int is_edge(int nodeIdx1, int nodeIdx2){
		return supergraph->is_edge(nodeIdx1, nodeIdx2);
	}

	//This function returns if the current graph is already a clr cluster.
	bool is_cluster(Param* p){
		//implement later
		return true;
	}

	/**********************************************************
	*This function removes a node given its index, a pointer to the id is returned.
	**********************************************************/
	T* remove_node(int idx){
		//first check if the node is in subgraph
		return NULL;
	}


	//This function removes a node given its id. If the node exists and is successfully removed then returns true, otherwise false
	bool remove_node(const T& node){
		return true;
	}

	/**********************************************************
	*This function searches a given node in the graph, if exists, the index is returns, otherwise -1 is returned.
	***********************************************************/
	int search_node(const T& node){
		for(int i=0;i<subgraphNodes.size();i++){
			if(&node == supergraph->get_node(subgraphNodes[i]))
				return subgraphNodes[i];
		}
		return -1;
	}
	~UndirectedCompSubgraph(){};
};



#endif /* CLR_SUBGRAPH_H_ */
