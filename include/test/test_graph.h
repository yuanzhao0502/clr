/*
 * test_graph.h
 *
 *  Created on: Mar 13, 2014
 *      Author: mac-97-41
 */

#ifndef TEST_GRAPH_H_
#define TEST_GRAPH_H_
#include<clr_graph.h>
#include<string>
#include<iostream>
/*In this header, tests of the classes and function sin clr_graph.h are written. The detailed source codes are implemented in
 * source/test/test_graph.cpp */

/*1.Test the constructors for UndirectedCompGraph*/
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


#endif /* TEST_GRAPH_H_ */
