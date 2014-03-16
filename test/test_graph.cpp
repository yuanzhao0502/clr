
#include<test/test_graph.h>

//Implement of test_UndirectedCompGraph
void test_UndirectedCompGraph(){
	//1. Read the graph
	//UndirectedCompGraph<std::string>* ucgraph1 = new
		//	UndirectedCompGraph<std::string>("/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_struct1",
			//"/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_id1");
	std::cout << "Start to read" <<std::endl;
	UndirectedCompGraph<std::string>* ucgraph1 = new UndirectedCompGraph<std::string>(
			"/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_struct1",
			"/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_id1"
			);
	std::cout << "Reading finished" <<std::endl;

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




