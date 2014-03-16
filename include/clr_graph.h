/*
 * clr_graph.h
 *
 *  Created on: Mar 14, 2014
 *      Author: mac-97-41
 */

#ifndef CLR_GRAPH_H_
#define CLR_GRAPH_H_
#include<clr_subgraph.h>
#include<clr_util.h>
#include<vector>
#include<fstream>

//An abstract(virtual) class for undirected graphs, specifiying ClrGraph
//Forward declaration of a template
/*
template<class T> class UndirectedSubgraph;

template<class T> class UndirectedGraph: public Graph<T>{
public:
	//this function returns the indexes of all the connected parts of the graph, in separated arrays
	virtual UndirectedSubgraph<T>** get_conn_subgraphs() = 0;
	//this function returns an array of the  indexes of all neighbouring nodes, given the index of the node
	virtual int* get_neighbours(int idx) = 0;
	//this function returns an array of the indexes of all neighbouring nodes, given the node
	virtual int* get_neighbours(T node) = 0;
	//This function returns if the current graph is already a clr cluster.
	virtual bool is_cluster(Param* p) = 0;
	//This function returns a graph_t struct to run metis
	virtual graph_t* get_graph_t() = 0;
	//virtual destructor
	virtual ~UndirectedGraph(){}
};
*/


//Declaration of the function to bread-first-search
template<class T>
//int* breadth_first_search(int toSearchIdx, UndirectedGraph<T>* graph);
int* breadth_first_search(int toSearchIdx, Graph<T>* graph);


//Graph for CLR based on matrix
template<class T>
//class UndirectedMatrixGraph: public UndirectedGraph<T>{
class UndirectedMatrixGraph: public Graph<T>{
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
	//This function adds node to the graph
	bool add_node(T node) = 0;
	//This function returns the array of ids in the graph
	//T* get_nodes() = 0;
	//This function get the number of nodes in the graph
	int get_node_count() = 0;
	//This function get the number of edges in the graph
	int get_edge_count() = 0;
	//This function returns the edge weight between two nodes given the indexes of the nodes
	double get_edge_weight(int idxNode1, int idxNode2) = 0;
	//This function returns the edge weight between two given nodes
	double get_edge_weight(T node1, T node2) = 0;
	//this function returns the indexes of all the connected parts of the graph, in separated arrays
	UndirectedCompSubgraph<T>** get_conn_subgraphs() = 0;
	//This functions returns if the current graph is connected, if connected, returns 1, else returns 0
	bool is_conn() = 0;
	//This function checks if there is an edge between two nodes, if connected, returns 1, else returns0
	int is_edge(int idxNode1, int idxNode2) = 0;
	//destructor
	~UndirectedMatrixGraph();
};


//Graph for CLR based on compressed storage format (CSR)s
//UndirectedCompGraph deriveds from UndirectedGraph
template<class T>
class UndirectedCompGraph:public Graph<T>{
	//id is a string array, storing the ids (or names) of all the nodes
	std::vector<T> nodes;

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
		nodes.assign(id, id+ sizeof (id)/sizeof (T));
		//Add assertions, invariable 1. the size of nodes must be equal to nvtxs in graph
		assert(nodes.size() == graph->nvtxs);

		//Assign xadj
		this->xadj.assign(graph->xadj, sizeof(graph->xadj)/sizeof(idx_t));

		//Add assertions, invariable 2. the size of xadj must be equal to nvtx+1
		assert(xadj.size() == sizeof (graph->xadj)/sizeof(idx_t));
		assert(xadj.size() == nodes.size()+1);

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
		std::cout << "UndirectedCompGraph with structFile and idFile is called"<<std::endl;
		Param* p = new Param(structFile, idFile);
		std::cout << p->get_metis_params()->filename<<std::endl; /* For test */
		//Then we read graph_t object from structFile
		graph_t* graph = ReadGraph(p->get_metis_params());

		//Next, we assign the three vectors, xadj, adjncy, adjwgt in this object
		//Several condition-checks are needed
		//1. Whether the id array has a length equal to nvtxs in graph
		//2. Whether the graph is properly initialized. Five elements are of the most importance: nvtxs, nedges, xadj, adjncy, adjwgt.

		/*Read ids from the id file */
		std::ifstream idfin(idFile);
		std::string id;
		while(idfin>> id){
			nodes.push_back(id);
		}
		std::cout<<"nodes "<<nodes.size()<<std::endl; /*For test*/
		//nodes.assign(id, id+ sizeof (id)/sizeof (T));
		//Add assertions, invariable 1. the size of nodes must be equal to nvtxs in graph
		assert(nodes.size() == graph->nvtxs);

		//Assign xadj
		this->xadj.assign(graph->xadj, graph->xadj+sizeof(graph->xadj)/sizeof(idx_t));

		//Add assertions, invariable 2. the size of xadj must be equal to nvtx+1
		std::cout <<"xadj  "<< xadj.size()<<std::endl; /*For test*/
		std::cout <<"graph_t xadj  "<<sizeof(graph->xadj)/sizeof(idx_t) <<std::endl; /*For test*、
		/*For test, output the content of xadj*/
		for(int i=0;i<8;i++){
			std::cout<<"graph_t xadj content  "<<graph->xadj[i]<<std::endl;
		}
		/*For test, really only 2 elements in graph->xadj ?*/
		std::cout<<"graph_t xadj content  "<<graph->xadj[2]<<std::endl;

		assert(xadj.size() == sizeof (graph->xadj)/sizeof(idx_t));

		//assign adjncy
		this->adjncy.assign(graph->adjncy, graph->adjncy+sizeof(graph->adjncy)/sizeof(idx_t));
		//invariable 3. the size of adjncy must be equal to 2* number of edges
		assert(adjncy.size() == sizeof (graph->adjncy)/sizeof(idx_t));
		assert(adjncy.size() == graph->nedges*2);

		//assign adjwgt
		this->adjwgt.assign(graph->adjwgt, graph->adjwgt+sizeof(graph->adjwgt)/sizeof(idx_t));
		//Add assertions, invariable 4. the size of adjwgt must be equal to the size of adjncy
		assert(adjwgt.size() == adjncy.size());
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
	 * This function removes the node with given index, and returns the id.
	 *********************************************************************************************/
	bool add_edge(int node1Idx, int node2Idx, double wgt){
		//This function is necessary for a graph package but unnecessary for this programme, we will
		//implement it later.
		return true;
	}

	/*********************************************************************************************
	*This function adds node to the graph
	*********************************************************************************************/
	bool add_node(const T& node){
		//We don't allow multiple nodes with same id, we have to make sure no duplicated nodes exist
		//If such id already exists, do nothing and return false;
		if (std::find(nodes.begin(), nodes.end(), node) == nodes.end())
			return false;
		nodes.push_back(node);

		//Add element in xadj, since no edge is incident to the newly added node yet, a new element is appended to
		//xadj with the value of adjncy -1;
		xadj.push_back(adjncy.size()-1);
		return true;
	}

	/*********************************************************************************************
	 * This function returns all the edges in the graph
	 *********************************************************************************************/
	int** get_edges(){
		//From i =0 to i = get_node_count() -1, we collect the neighbor indexes larger than i
		//(that is to avoid duplicated edges)

		//Init a vector of int*
		std::vector<int*> edges;
		for(int i=0;i< this->get_node_count();i++){

			//Check all the neighboring edges
			for(int j = this->xadj[i];j<this->xadj[i+1];j++){
				//Init an array of two indexes as an edge
				int* e = new int[2];
				//If the neighboring index is smaller than i, then we add it to edges.
				if(j < i){
					e[0] = i;
					e[1] = j;
					edges.push_back(e);
				}
			}
		}
		return &edges[0];
	}

	/*********************************************************************************************
	 * This function get the number of nodes in the graph
	*********************************************************************************************/
	int get_node_count(){
		return nodes.size();
	}

	/*********************************************************************************************
	*This function returns the array of ids in the graph
	*********************************************************************************************/
	const T* get_node(int nodeIdx){
		//Check the index
		if(nodeIdx <0 || nodeIdx >= nodes.size())
			return NULL;
		return &nodes[nodeIdx];
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
	double get_edge_weight(int idx1, int idx2){
		//First, we search if the two nodes are connected
		//Here we compare the number of nodes connected to Node1 and Node2, and we search the node with less neighbours
		int startIdx, endIdx, neighborIdx;
		if(xadj[idx1+1] - xadj[idx1] >= xadj[idx2+1] - xadj[idx2]){
			startIdx = xadj[idx2];
			endIdx = xadj[idx2+1]-1;
			neighborIdx = idx1;
		}else{
			startIdx = xadj[idx1];
			endIdx = xadj[idx1+1] -1;
			neighborIdx = idx2;
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
	double get_edge_weight(const T& node1, const T& node2) {
		//First, search the two nodes, to check if they do exist in the graph
		int node1Idx = std::find(nodes.begin(), nodes.end(), node1) - nodes.begin();
		int node2Idx = std::find(nodes.begin(), nodes.end(), node2) - nodes.begin();
		//if any of the nodes is unfound, return 0
		if(node1Idx >= nodes.size() || node2Idx >= nodes.size())
			return 0;
		return get_edge_weight(node1Idx, node2Idx);
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
		if(nodeIdx <0 || nodeIdx>= nodes.size())
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
		int idx = (int)(find(nodes.begin(),nodes.end(), node)- nodes.begin());
			//	== nodes.size() ?
				//ans = NULL:
				//ans = get_neighbours(idx);
		if(idx == nodes.size())
			return NULL;
		return get_neighbours(idx);

	}

	/*********************************************************************************************
	 * This function returns the indexes of
	 * all the connected parts of the graph, in separated arrays
	 *********************************************************************************************/
	 Subgraph<T>** get_conn_subgraphs(){
		 //This bool array indicates whether the given node is visited
		 bool boolVisitedArr[this->get_node_count()];
		 //init boolVisitedArr
		 for(int i=0;i<this->get_node_count();i++)
			 boolVisitedArr[i] = false;

		 //this vector stores all subgraphs
		 std::vector<Subgraph<T>*> subgraphVec;

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
			 Subgraph<T>* sub1 = new UndirectedCompSubgraph<T>(breadth_first_search(firstUnvisitedIdx, this), this);
			 subgraphVec.push_back(sub1);
		 }
		return &subgraphVec[0];
	}

	/*********************************************************************************************
	 * This function returns if the current graph is connected, if connected, returns 1, else returns 0
	 *********************************************************************************************/
	bool is_conn(){
		if(sizeof(breadth_first_search(0,this))/sizeof(int) == this->get_node_count())
			return true;
		else return false;
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

	/***********************************************************************************************
	 * This function returns if the graph is already a cluster. It's not yet written
	 *********************************************************************************************/
	bool is_cluster(Param* p){
		return true;
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
	bool remove_node(const T& node){
		//First search for node
		int idx = search_node(node);
		if(idx <0)
			return false;
		//This function is necessary for a graph package but unnecessary for this programme, we will
		//implement it later.
		//Pseudo-code
		//Create another three vec, xadj_new, adjncy_new and adjwgt_new
		//1. Remove neighbours in xadj for node idx and remove the corresponding elements in adjncy
		//2. Change the values for all xadj[i] i>= idx.
		//3. Remove all adjncy that is incident to idx, change xadj.
		//4. All nodes with indice larger than idx should minus 1
		return false;
	}


	/*********************************************************************************************
	 *This function searches for one node in the graph and returns the index of the given node.
	 *If the node is not found, -1 is returned
	 *********************************************************************************************/
	int search_node(const T& node){
		int position =  find(this->nodes.begin(),this->nodes.end(),node)
				- this->nodes.begin();
		if(position == this->get_node_count())
			return -1;
		else return position;
	}
	//destructor
	~UndirectedCompGraph(){};
};


//An abstract(virtual) class for directed graphs, specifying ClrGraph
//Forward declaration of a template
/*
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
	virtual DirectedSubgraph<T>** get_conn_subgraphs() = 0;
	//virtual destructor
	virtual ~DirectedGraph(){};
};
*/


//This is the directed graph based on Lemon package
template<class T>
//class DirectedLemonGraph:public DirectedGraph<T>{
class DirectedLemonGraph: public Graph<T>{
	std::vector<T> ids;
	lemon::ListDigraph* lemonGraph;
	std::vector<lemon::ListDigraph::Node> lemonNodes;
	lemon::ListDigraph::ArcMap<double>* capacityMap;
public:
	//constructor
	/* Create a directed graph from two undirected subgraphs, according to the given algorithm*/
	//DirectedLemonGraph(UndirectedSubgraph<T> subgraph1, UndirectedSubgraph<T> subgraph2){
	DirectedLemonGraph(Subgraph<T> subgraph1, Subgraph<T> subgraph2){
		//Check which subgraph is the the smaller one
		//UndirectedSubgraph<T>* smallSubgraph;
		Subgraph<T>* smallSubgraph;
		subgraph1.get_node_count() < subgraph2.get_node_count() ? smallSubgraph = &subgraph1 :
				smallSubgraph = &subgraph2;
		//Compute the conductance, which is the sum of all cross-subgraphs edge weights
		double condc = 0;
		for(int i=0;i<subgraph1.get_node_count();i++)
			for(int j=0;j<subgraph2.get_node_count();j++)
				condc += subgraph1.get_super_graph()->get_edge_weight(subgraph1.get_super_index(i),
						subgraph2.get_super_index(j));

		//construct the new directed graph, including the source and sink nodes, storing
		//at the end of the node vector
		lemonGraph = new lemon::ListDigraph();
		for(int i=0;i<smallSubgraph->get_node_count()+2;i++)
			lemonNodes.push_back(lemonGraph->addNode());
		//Init the arc map wgtMap
		capacityMap = lemon::ListDigraph::ArcMap<double>(*lemonGraph);
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
			*capacityMap[lemonGraph->lemon::ListDigraph::addArc(lemonNodes[smallSubgraph->get_node_count()-2],
					lemonNodes[i])] = smallSubgraph->get_node_count();
		}

		//3.Add a single directed edge from every node in smallSubgraph to the sink, with capacity of condc
		for(int i=0;i<smallSubgraph->get_node_count();i++){
					*capacityMap[lemonGraph->addArc(lemonNodes[i],
							lemonNodes[smallSubgraph->get_node_count()-1])] = condc;
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


/*********************************************************************************************
*1. Breadth first search
**********************************************************************************************/

template<class T>
//int* breadth_first_search(int toSearchIdx, UndirectedGraph<T>* graph){
int* breadth_first_search(int toSearchIdx, Graph<T>* graph){
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
	return &idxVisitedVec[0];
}






#endif /* CLR_GRAPH_H_ */
