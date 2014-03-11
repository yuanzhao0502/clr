/*
 * clr_graph.h
 *
 *  Created on: Mar 3, 2014
 *      Author: mac-97-41
 */

#ifndef CLR_GRAPH_H_
#define CLR_GRAPH_H_
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

//A general class for the graphs in package CLR
template<class T>
class Graph{

	//This function returns the array of ids in the graph
	virtual T* get_nodes() = 0;
	//This function returns all edges in a two-dimensional array
	virtual int** get_edges() = 0;
	//This function adds node to the graph
	//If successful, returns the index of the inserted node, otherwise returns -1
	virtual bool add_node(const T& node) = 0;
	//This function adds an edge to the graph, given the indices of two nodes and the edge weight
	//If successful, returns true, otherwise false
	virtual bool add_edge(int node1Idx, int node2Idx, double wgt) = 0;
	//This function searches a given node in the graph, if exists, the index is returns, otherwise -1 is returned.
	virtual int search_node(const T& node) = 0;
	//This function removes a node given its index, a pointer to the id is returned.
	virtual T* remove_node(int idx) = 0;
	//This function removes a node given its id. If the node exists and is successfully removed then returns true, otherwise false
	virtual bool remove_node(const T& node) = 0;
	//This function returns the pointer to a node in constant mode.
	virtual const T* get_node(int nodeIdx) = 0;
	//This function get the number of nodes in the graph
	virtual int get_node_count() = 0;
	//This function get the number of edges in the graph
	virtual int get_edge_count() = 0;
	//This function returns the edge weight between two nodes given the indexes of the nodes
	virtual double get_edge_weight(int nodeIdx1, int nodeIdx2) = 0;
	//This function returns the edge weight between two given nodes
	virtual double get_edge_weight(const T& node1, const T& node2) = 0;
	//This functions returns if the current graph is connected, if connected, returns 1, else returns 0
	virtual bool is_conn() = 0;

	//This function checks if there is an edge between two nodes, if connected, returns 1, else returns0
	virtual int is_edge(int nodeIdx1, int nodeIdx2) = 0;
	//the virtual destructor
	virtual ~Graph(){}

};

//An abstract(virtual) class for directed graphs, specifying ClrGraph
//Forward declaration of a template
template<class T>
class DirectedSubgraph;
//Definition of the class DirectedGraph
template<class T>
class DirectedGraph:public Graph<T>{
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
	//this function returns the indexes of all the connected parts of the graph, in separated arrays
	virtual DirectedSubgraph<T>* get_conn_subgraphs() = 0;
	//virtual destructor
	virtual ~DirectedGraph(){};
};


//An abstract(virtual) class for undirected graphs, specifiying ClrGraph
//Forward declaration of a template
template<class T> class UndirectedSubgraph;
template<class T>
class UndirectedGraph: public Graph<T>{
	//this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	virtual int* get_neighbours(int idxNode) = 0;
	//this function returns an array of the indexes of all neighbouring nodes, given the node
	virtual int* get_neighbours(T node) = 0;
	//this function returns the indexes of all the connected parts of the graph, in separated arrays
	virtual UndirectedSubgraph<T>* get_conn_subgraphs() = 0;
	//This function returns if the current graph is already a clr cluster.
	virtual bool is_clr_cluster(Param* p) = 0;
	//virtual destructor
	//This function returns a graph_t struct to run metis
	virtual graph_t* get_graph_t() = 0;
	virtual ~UndirectedGraph(){}
};





//Graph for CLR based on matrix
template<class T>
class UndirectedMatrixGraph: public UndirectedGraph<T>{
	//The number of nodes and edges
	idx_t nvtxs, nedges;
	//the id (name) string array for all nodes
	std::vector<T> id;
	//a two dimension double array to store the edge weights
	double** adjwgt;
	/*
	 * The constructor for MatrixClrGraph with 3 arguments: nvtxs, nedges, id and adjwgt
	 */
public:
	UndirectedMatrixGraph (idx_t nvtxs, idx_t nedges, T* id, double** adjwgt);
	/*********************************************************************************************
	 * This constructor reads the matrix from a file. Make sure that nvtxs, nedges, id and adjwgt are initialized.
	 *
	 * Since in this constructor we are gonna use for loop, it's not suitable to be an inline function, we put it elsewhere.
	 *********************************************************************************************/
	UndirectedMatrixGraph(const std::string& filePath);
	//This function returns the array of ids in the graph
	T* get_nodes() = 0;
	//This function adds node to the graph
	bool add_node(T node) = 0;
	//This function get the number of nodes in the graph
	int get_node_count() = 0;
	//This function get the number of edges in the graph
	int get_edge_count() = 0;
	//This function returns the edge weight between two nodes given the indexes of the nodes
	double get_edge_weight(int idxNode1, int idxNode2) = 0;
	//This function returns the edge weight between two given nodes
	double get_edge_weight(T node1, T node2) = 0;
	//This functions returns if the current graph is connected, if connected, returns 1, else returns 0
	bool is_conn() = 0;
	//this function returns the indexes of all the connected parts of the graph, in separated arrays
	int** get_conn_subgraphs() = 0;
	//This function checks if there is an edge between two nodes, if connected, returns 1, else returns0
	int is_edge(int idxNode1, int idxNode2) = 0;
	//destructor
	~UndirectedMatrixGraph();
};


//Graph for CLR based on compressed storage format (CSR)s
//Forward declaration of UndirectedSubCompGraph
template<class T> class UndirectedCompSubgraph;
//UndirectedCompGraph deriveds from UndirectedGraph
template<class T>
class UndirectedCompGraph : public UndirectedGraph<T>{
	//id is a string array, storing the ids (or names) of all the nodes

	std::vector<T> ids;

	/*********************************************************************************************
	 * These three  variables stores the needed information in graph_t
	 *********************************************************************************************/
	std::vector<idx_t> xadj;		/* Pointers to the locally stored vertices */
	std::vector<idx_t> adjncy;   /* Array that stores the adjacency lists of nvtxs */
	std::vector<idx_t> adjwgt;   /* Array that stores the weights of the adjacency lists */

	friend class UndirectedCompSubgraph<T>; /*UndirectedCompSubgraph is a friend class. It must have the access to the class members*/
	/*********************************************************************************************
	 * Constructor with two arguments.
	 * id: The string array of all ids
	 * graph*: the pointer to the graph_t struct
	 *********************************************************************************************/
public:
	UndirectedCompGraph (const T* id, const graph_t* graph){

		//Several condition-checks are needed
		//1. Whether the id array has a length equal to nvtxs in graph
		//2. Whether the graph is properly initialized. Five elements are of the most importance: nvtxs, nedges, xadj, adjncy, adjwgt.
		ids.assign(id, id+ sizeof (id)/sizeof (T));
		//Add assertions, invariable 1. the size of nodes must be equal to nvtxs in graph
		assert(ids.size() == graph->nvtxs);

		//Assign xadj
		this->xadj.assign(graph->xadj, sizeof(graph->xadj)/sizeof(idx_t));

		//Add assertions, invariable 2. the size of xadj must be equal to nvtx+1
		assert(xadj.size() == sizeof (graph->xadj)/sizeof(idx_t));
		assert(xadj.size() == ids.size()+1);

		//assign adjncy
		this->adjncy.assign(graph->adjncy, sizeof(graph->adjncy)/sizeof(idx_t));
		//invariable 3. the size of adjncy must be equal to 2* number of edges
		assert(adjncy.size() == sizeof (graph->adjncy)/sizeof(idx_t));
		assert(adjncy.size() == graph->nedges*2);


		//assign adjwgt
		this->adjwgt.assing(graph->adjwgt, sizeof(graph->adjwgt)/sizeof(idx_t));
		//Add assertions, invariable 4. the size of adjwgt must be equal to the size of adjncy
		assert(adjwgt.size() == adjncy.size());
	}
	/*********************************************************************************************
	 * This constructor reads two files, one providing the id array, the other giving the structure
	 *********************************************************************************************/
	UndirectedCompGraph( const std::string& structFile,const std::string& idFile){
		//We read an UndirectedCompGraph from a file whose format is in accordance with the rule in metis manual
		//We do it by using the function "ReadGraph()" defined in metis
		//First, we create a Param object, with structFile(graphFile) and idFile assigned.
		Param* p = new Param(structFile, idFile);

		//Then we read graph_t object from structFile
		graph_t* graph = ReadGraph(p->get_metis_params());

		//Next, we assign the three vectors, xadj, adjncy, adjwgt in this object
		//Several condition-checks are needed
		//1. Whether the id array has a length equal to nvtxs in graph
		//2. Whether the graph is properly initialized. Five elements are of the most importance: nvtxs, nedges, xadj, adjncy, adjwgt.

		/*Read ids from the id file */
		//nodes.assign(id, id+ sizeof (id)/sizeof (T));
		//Add assertions, invariable 1. the size of nodes must be equal to nvtxs in graph
		assert(ids.size() == graph->nvtxs);

		//Assign xadj
		this->xadj.assign(graph->xadj, sizeof(graph->xadj)/sizeof(idx_t));
		//Add assertions, invariable 2. the size of xadj must be equal to nvtx+1
		assert(xadj.size() == sizeof (graph->xadj)/sizeof(idx_t));
		assert(xadj.size() == ids.size()+1);

		//assign adjncy
		this->adjncy.assign(graph->adjncy, sizeof(graph->adjncy)/sizeof(idx_t));
		//invariable 3. the size of adjncy must be equal to 2* number of edges
		assert(adjncy.size() == sizeof (graph->adjncy)/sizeof(idx_t));
		assert(adjncy.size() == graph->nedges*2);

		//assign adjwgt
		this->adjwgt.assing(graph->adjwgt, sizeof(graph->adjwgt)/sizeof(idx_t));
		//Add assertions, invariable 4. the size of adjwgt must be equal to the size of adjncy
		assert(adjwgt.size() == adjncy.size());


		//Finally, we read idFile for ids
		std::ifstream fin(idFile);
		std::string id;
		while(fin >> id)
			ids.push_back(id);
	}


	/*********************************************************************************************
	 * This constructor accepts a param_t as argument, reading the CSR from the path given in the param_t, and read id string array
	 * from another file
	 *********************************************************************************************/
	UndirectedCompGraph(Param& p){
		//First, we create a graph_t file from the params_t object in p
		graph_t* graph = ReadGraph(p.get_metis_params());
	}



	/*Implementing the virtual functions in Graph<T> */
	/*********************************************************************************************
	 * This function get the number of nodes in the graph
	*********************************************************************************************/
	int get_node_count(){
		return ids.size();
	}

	/*********************************************************************************************
	*This function returns the array of ids in the graph
	*********************************************************************************************/
	T* get_nodes(){
		return &ids[0];
	}


	/*********************************************************************************************
	*This function returns the array of ids in the graph
	*********************************************************************************************/
	const T* get_node(int nodeIdx){
		//Check the index
		if(nodeIdx <0 || nodeIdx >= ids.size())
			return NULL;
		return ids.begin()+nodeIdx;
	}


	/*********************************************************************************************
	*This function adds node to the graph
	*********************************************************************************************/
	int add_node(T node){
		//We don't allow multiple nodes with same id, we have to make sure no duplicated nodes exist
		//If such id already exists, do nothing and return false;
		if (std::find(ids.begin(), ids.end(), node) == ids.end())
			return false;
		ids.push_back(node);

		//Add element in xadj, since no edge is incident to the newly added node yet, a new element is appended to
		//xadj with the value of adjncy -1;
		xadj.push_back(adjncy.size()-1);
		return ids.size()-1;
	}

	/*********************************************************************************************
	 * This function get the number of edges in the graph
	 *********************************************************************************************/
	int get_edge_count(){
		return adjncy.size()/2;
	}

	/*********************************************************************************************
	 * This function returns the edge weight between two nodes given the indexes of the nodes
	 *********************************************************************************************/
	double get_edge_weight(int node1Idx, int node2Idx){
		//First, we search if the two nodes are connected
		//Here we compare the number of nodes connected to Node1 and Node2, and we search the node with less neighbours
		int startIdx, endIdx, neighborIdx;
		if(xadj[node1Idx+1] - xadj[node1Idx] >= xadj[node2Idx+1] - xadj[node2Idx]){
			startIdx = xadj[node2Idx];
			endIdx = xadj[node2Idx+1]-1;
			neighborIdx = node1Idx;
		}else{
			startIdx = xadj[node1Idx];
			endIdx = xadj[node1Idx+1] -1;
			neighborIdx = node2Idx;
		}

		//search in adjncy
		for(int i = startIdx;i<=endIdx ;i++){
			//if the two nodes are connected
			if(adjncy[i] == neighborIdx)
				return adjwgt[i];
		}
		//if no node matches, then the two nodes are not connected
		return 0;
	}


	/*********************************************************************************************
	 * This function returns the edge weight between two given nodes
	 *********************************************************************************************/
	double get_edge_weight(T node1, T node2) {
		//First, search the two nodes, to check if they do exist in the graph
		int node1Idx = std::find(ids.begin(), ids.end(), node1) - ids.begin();
		int node2Idx = std::find(ids.begin(), ids.end(), node2) - ids.begin();
		//if any of the nodes is unfound, return 0
		if(node1Idx >= ids.size() || node2Idx >= ids.size())
			return 0;
		return get_edge_weight(node1Idx, node2Idx);
	}


	/*********************************************************************************************
	 * This function returns if the current graph is connected, if connected, returns 1, else returns 0
	 *********************************************************************************************/
	bool is_conn(){
		UndirectedCompSubgraph<T>* subgraph = breadth_first_search(0,this);
		if(sizeof(subgraph)/sizeof(UndirectedCompSubgraph<T>) == this->get_node_count())
			return true;
		else return false;
	}

	/*********************************************************************************************
	 * This function removes the node with given index, and returns the id.
	 *********************************************************************************************/
	T* remove_node(int idx){
		//This function is necessary for a graph package but unnecessary for this programme, we will
		//implement it later.
		//Pseudo-code
		//Create another three vec, xadj_new, adjncy_new and adjwgt_new
		//1. Remove neighbours in xadj for node idx and remove the corresponding elements in adjncy
		//2. Change the values for all xadj[i] i>= idx.
		//3. Remove all adjncy that is incident to idx, change xadj.
		//4. All nodes with indice larger than idx should minus 1
		return NULL;
	}

	/*********************************************************************************************
	 * This function removes the node with given index, and returns the id.
	 *********************************************************************************************/
	T* remove_node(const T& node){
		//First search for node
		int idx = search_node(node);
		if(idx <0)
			return NULL;
		//This function is necessary for a graph package but unnecessary for this programme, we will
		//implement it later.
		//Pseudo-code
		//Create another three vec, xadj_new, adjncy_new and adjwgt_new
		//1. Remove neighbours in xadj for node idx and remove the corresponding elements in adjncy
		//2. Change the values for all xadj[i] i>= idx.
		//3. Remove all adjncy that is incident to idx, change xadj.
		//4. All nodes with indice larger than idx should minus 1
		return NULL;
	}


	/*********************************************************************************************
	 * This function removes the node with given index, and returns the id.
	 *********************************************************************************************/
	bool add_edge(int node1Idx, int node2Idx, double wgt){
		//This function is necessary for a graph package but unnecessary for this programme, we will
		//implement it later.
		return true;
	}

	/*********************************************************************************************
	 * This function checks if there is an edge
	 * between two nodes, if connected, returns 1, else returns 0
	 *********************************************************************************************/
	int is_edge(int idxNode1, int idxNode2){
		//First, we search if the two nodes are connected
		//Here we compare the number of nodes connected to Node1 and Node2, and we search the node with less neighbours
		int startIdx, endIdx, neighborIdx;
		if(xadj[idxNode1+1] - xadj[idxNode1] >= xadj[idxNode2+1] - xadj[idxNode2]){
			startIdx = xadj[idxNode2];
			endIdx = xadj[idxNode2+1]-1;
			neighborIdx = idxNode1;
		}else{
			startIdx = xadj[idxNode1];
			endIdx = xadj[idxNode1+1] -1;
			neighborIdx = idxNode2;
		}

		//search in adjncy
		for(int i = startIdx;i<=endIdx ;i++){
			//if the two nodes are connected, we return 1
			if(adjncy[i] == neighborIdx)
				return 1;
		}
		//if no node matches, then the two nodes are not connected
		return 0;
	}

	//This function returns a pointer to struct graph_t
	graph_t* get_graph_t(){
		graph_t* graph = new graph_t();
		//assign the value xadj, adjncy and adjwgt
		graph->adjncy = &this->adjncy[0];
		graph->adjwgt = &this->adjwgt[0];
		graph->xadj = &this->xadj[0];
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

	//Implementing the virtual functions derived from UndirectedGraphs
	/*********************************************************************************************
	 *this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	 *********************************************************************************************/
	int* get_neighbours(int nodeIdx){
		//check if nodeIdx is within the range of nodes, if yes, return NULL
		if(nodeIdx <0 || nodeIdx>= ids.size())
			return NULL;

		//Get the start index and end index in adjncy
		int startIdx = xadj[nodeIdx], endIdx = xadj[nodeIdx+1]-1;
		int* res= new int[endIdx - startIdx +1];
		for (int i=startIdx;i<=endIdx;i++){
			res[i-startIdx] = adjncy[i];
		}
		return res;
	}


	/*********************************************************************************************
	 * This function returns an array of the indexes of all neighbouring nodes, given the node
	 **********************************************************************************************/
	int* get_neighbours(T node){
		//Check if this node exists. If not, return NULL
		int* ans ;
		int idx = find(ids.begin(),ids.end(), node)- ids.begin()  == ids.size() ? ans = NULL:
				ans = get_neighbours(idx);
		return ans;

	}


	/*********************************************************************************************
	 * This function returns the indexes of
	 * all the connected parts of the graph, in separated arrays
	 *********************************************************************************************/
	 UndirectedCompSubgraph<T>* get_conn_subgraphs(){
		 //This bool array indicates whether the given node is visited
		 bool boolVisitedArr[this->get_node_count()];
		 //init boolVisitedArr
		 for(int i=0;i<this->get_node_count();i++)
			 boolVisitedArr[i] = false;

		 //this vector stores all subgraphs
		 std::vector<UndirectedCompSubgraph<T>*> subgraphVec;

		while(true){
			 //firstunvisitedIdx stores the index of the first unvisited node
			 int firstUnvisitedIdx =-1;
			 for(int i=0;i<this->get_node_count();i++)
				 if(!boolVisitedArr[i])
					 firstUnvisitedIdx = i;
			 //If all nodes have been visited,then break
			 if(firstUnvisitedIdx ==-1)
				 break;
			 //Use breadth-first search to get all connected nodes and create a UndirectedCompSubgraph subject
			 subgraphVec.push_back(new UndirectedCompSubgraph<T>(breadth_first_search(firstUnvisitedIdx, this)));
		 }
		return subgraphVec.begin();
	}
	//destructor

	~UndirectedCompGraph();
};


//This class gives the interface of the most general subgraph
template<class T>
class Subgraph: public Graph<T>{
	//Constructor
	//This function returns a pointer to the super graph
	virtual Graph<T>* get_supergraph() = 0;
	//This function returns
	virtual Graph<T>* get_super_index(int idx)=0;
	//virtual destructor
	virtual ~Subgraph(){};
};

//This class gives some declarations of the functions in a directed subgraph
template<class T>
class DirectedSubgraph: public Subgraph<T>{

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

//This class gives some declarations of the function in a undirected subgraph
template<class T>
class UndirectedSubgraph: Subgraph<T>{
	//this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	virtual int* get_neighbours(int idxNode) = 0;
	//this function returns an array of the indexes of all neighbouring nodes, given the node
	virtual int* get_neighbours(T node) = 0;
	//virtual destructor
	virtual ~UndirectedSubgraph(){}
	//this function returns
};



//This class gives the definition of UndirectedCompSubgraph, implementing all virtual functions
template<class T>
class UndirectedCompSubGraph: public UndirectedSubgraph<T>{
	std::vector<int> subgraphNodes;
	UndirectedCompSubgraph<T>* superGraph;

public:
	//constructor
	/**********************************************************
	 * This constructor creates a UndirectedCompSubGraph object with a node array and a pointer to the super graph
	 **********************************************************/
	UndirectedCompSubGraph(int* nodes, UndirectedGraph<T> superGraph){
		this->subgraphNodes.assign(nodes,sizeof(nodes)/sizeof(int));
		this->superGraph = superGraph;
	}


	//Implement the virtual functions derived from Graph<T>
	/**********************************************************
	*This function returns the array of ids in the graph
	**********************************************************/
	T* get_nodes() {
		//create an array of ids
		T* idArr[subgraphNodes.size()];
		for(int i=0;i<subgraphNodes.size();i++)
			idArr[i] = superGraph->get_node(subgraphNodes[i]);
		return idArr;
	}


	/**********************************************************
	*This function adds node to the graph.
	*If successful, returns the index of the inserted node, otherwise returns -1
	***********************************************************/
	int add_node(const T& node){
		subgraphNodes.push_back(superGraph->add_node(node));
		return subgraphNodes[subgraphNodes.size()-1];
	}

	/**********************************************************
	*This function searches a given node in the graph, if exists, the index is returns, otherwise -1 is returned.
	***********************************************************/
	int search_node(const T& node){
		for(int i=0;i<subgraphNodes.size();i++){
			if(node == superGraph->get_node(subgraphNodes[i]))
				return subgraphNodes[i];
		}
		return -1;
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

	//This function returns the id of a node given an index
	const T* get_node(int nodeIdx){
		//Check nodeIdx
		if(nodeIdx <0 || nodeIdx >= superGraph->get_node_count())
			return NULL;

		//Check if the nodeIdx is in subgraph
		if(find(subgraphNodes.begin(), subgraphNodes.end(), nodeIdx) == subgraphNodes.end())
			return NULL;
		return superGraph->get_nodes()+nodeIdx;
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
		return superGraph->get_edge_weight(nodeIdx1, nodeIdx2);
	}

	//This function returns the edge weight between two given nodes
	double get_edge_weight(const T& node1, const T& node2){
		return superGraph->get_edge_weight(node1,node2);
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
		return superGraph->is_edge(nodeIdx1, nodeIdx2);
	}


	//Implementing the virtual functions derived from UndirectedGraphs
	/*********************************************************************************************
	 *this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	 *********************************************************************************************/
	int* get_neighbours(int nodeIdx){
		std::vector<int> superGraphNeighbours;
		int* superGraphNeighboursArr = superGraph->get_neighbours();
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
			copy(superGraph->adjncy.begin()+superGraph->xadj[subgraphNodes[i]],
					superGraph->adjncy.begin()+superGraph->xadj[subgraphNodes[i]+1], sub_adjncy.end()-1);
			copy(superGraph->adjwgt.begin()+superGraph->xadj[subgraphNodes[i]],
					superGraph->adjwgt.begin()+superGraph->xadj[subgraphNodes[i]+1], sub_adjwgt.end()-1);
			//Write to sub_xadj
			/*If the size of sub_xadj is equal to 0, then we know the first node starts from index 0 in the adjncy*/
			if(sub_xadj.size() == 0)
				sub_xadj.push_back(0);
			/*Else, we push back the element "number of neighbours + start index"*/
			else
				sub_xadj.push_back(sub_xadj[sub_xadj.size()-1] +
						superGraph->xadj[subgraphNodes[i]+1]-superGraph->xadj[subgraphNodes[i]]);

		}
		//construct a graph_t object
		graph_t* graph = new graph_t();
		//assign the value xadj, adjncy and adjwgt
		graph->adjncy = &this->sub_adjncy[0];
		graph->adjwgt = &this->sub_adjwgt[0];
		graph->xadj = &this->sub_xadj[0];
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

};


//This is the directed graph based on Lemon package
template<class T>
class DirectedLemonGraph:public DirectedGraph<T>{
	std::vector<T> ids;
	lemon::ListDigraph* lemonGraph;
	std::vector<lemon::ListDigraph::Node> lemonNodes;
	lemon::ListDigraph::ArcMap<double>* capacityMap;
public:
	//constructor
	/* Create a directed graph from two undirected subgraphs, according to the given algorithm*/
	DirectedLemonGraph(UndirectedSubgraph<T> subgraph1, UndirectedSubGraph<T> subgraph2){
		//Check which subgraph is the the smaller one
		UndirectedSubgraph<T>* smallSubgraph;
		subgraph1.get_node_count() < subgraph2.get_node_count() ? smallSubgraph = &subgraph1 :
				smallSubgraph = &subgraph2;
		//Compute the conductance, which is the sum of all cross-subgraphs edge weights
		doubel condc = 0;
		for(int i=0;i<subgraph1.get_node_count();i++)
			for(int j=0;j<subgraph2.get_node_count();j++)
				condc += subgraph1.get_super_graph()->get_edge_weight(subgraph1.get_super_index(i),
						subgraph2.get_super_index(j));

		//construct the new directed graph, including the source and sink nodes, storing
		//at the end of the node vector
		lemonGraph = new ListGraph();
		for(int i=0;i<smallSubgraph->get_node_count()+2;i++)
			lemonNodes.push_back(lemonGraph->addNode());
		//Init the arc map wgtMap
		capacityMap = ArcMap<double>(*lemonGraph);
		//Add the arcs
		//1.Replace every edge that connects a pair of smallSubgraph nodes with a pair of edges,
		//each of which points to the other with the capacity of smallSubgraph->get_edge_weight(i,j)
		for(int i=0;i<smallSubgraph->get_node_count();i++)
			for(int j=0;j<smallSubgraph->get_node_count();j++)
				if(smallSubgraph->is_edge(i,j)){
					*capacityMap[lemonGraph->addArc(lemonNodes[i],lemonNodes[j])] = smallSubgraph->get_edge_weight(i,j);
				}

		//2.Add a single directed edge from source node (second node from rear) to each node in the smallSubgraph,
		//with capacity of smallSubGraph->get_node_count()
		for(int i=0;i<smallSubgraph->get_node_count();i++){
			*capacityMap[lemonGraph->addArc(lemonNodes[smallSubGraph->get_node_count()-2],
					lemonNodes[i])] = smallSubgraph->get_node_count();
		}

		//3.Add a single directed edge from every node in smallSubgraph to the sink, with capacity of condc
		for(int i=0;i<smallSubgraph->get_node_count();i++){
					*capacityMap[lemonGraph->addArc(lemonNodes[i],
							lemonNodes[smallSubGraph->get_node_count()-1])] = condc;
				}
	}


	lemon::ListDigraph::Node& get_source_node(){
		return lemonNodes[lemonNodes.size()-2];
	}

	lemon::ListDigraph::Node& get_sink_node(){
		return lemonNodes[lemonNodes.size()-1];
	}

	lemon::ListDigraph::ArcMap<double>& get_capacity_map(){
		return capacityMap;
	}

	std::vector<lemon::ListDigraph::Node>& get_lemon_nodes(){
		return lemonNodes;
	}


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
	bool add_subgraph(Subgraph<T>* subgraph){
		clusters.push_back(subgraph);
		return true;
	}
	~Cluster(){}
};


//Some util functions
/*********************************************************************************************
*1. Breadth first search
**********************************************************************************************/
template<class T>
int* breadth_first_search(int toSearchIdx, UndirectedGraph<T>* graph){
	//First check if the given index is in the graph
	if(toSearchIdx <0 || toSearchIdx >= graph->get_node_count())
		return NULL;
	//isVisistedVec stores the indices which bfs has visited. toSearchVec stores the indices to search
	std::vector<int> idxVisitedVec;
	std::deque<int> toSearchVec;

	//boolVisitedArr is an array stores size() bool values, each indicating whether the corrsponding node is visited or not
	bool boolVisitedArr[graph->get_node_count()];
	//init boolVisitedArr
	for(int i=0;i<graph->get_node_count();i++){
		boolVisitedArr[i] = false;
	}

	idxVisitedVec.push_back(toSearchIdx);
	toSearchVec.push_back(toSearchIdx);

	while(!toSearchVec.empty()){
		//get the index to search from the front of the array
		int index = toSearchVec.front();
		toSearchVec.pop_front();
		int* neighbours = graph->get_neighbours(index);
		//for all unvisited nodes
		for(int i=0;i<sizeof(neighbours)/sizeof(int) ;i++){
			if(boolVisitedArr[neighbours[i]] == false){
				boolVisitedArr[neighbours[i]] =true;
				idxVisitedVec.push_back(neighbours[i]);
				toSearchVec.push_back(neighbours[i]);
			}
		}
	}
}


template<class T>
double get_partition_score(UndirectedSubgraph<T>* subgraph1, UndirectedSubgraph<T>* subgraph2){
	//need implementation

	return 1;
}



#endif /* CLR_GRAPH_H_ */
