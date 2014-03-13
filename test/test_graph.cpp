/*
 * test_graph.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: penpen926
 */
#include<test/test_graph.h>
//Implement of test_UndirectedCompGraph
void test_UndirectedCompGraph(){
	//1. Read the graph
	UndirectedCompGraph<std::string>* ucgraph1 = new
			UndirectedCompGraph("/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_struct1",
			"/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_id1");
	int nodeCount = ucgraph1->get_node_count();
	int edgeCount = ucgraph1->get_edge_count();
	for(int i=0;i<nodeCount;i++){
		std::cout << ucgraph1->get_node(i)<<"  "<<std::endl;
	}

	//get all edges
	int** edges = ucgraph1->get_edges();
	for(int i=0;i<edgeCount;i++){
		std::cout<< edges[i][0]<<"   "<<edges[i][1]<<std::endl;
	}
	delete(ucgraph1);


}




