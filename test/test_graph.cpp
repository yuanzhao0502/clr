
#include<test/test_graph.h>
#include<metis_function.h>
//Implement of test_UndirectedCompGraph
void test_UndirectedCompGraph(){
	//1. Read the graph
	//UndirectedCompGraph<std::string>* ucgraph1 = new
		//	UndirectedCompGraph<std::string>("/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_struct1",
			//"/Users/penpen926/Work/clustering_local_restr/clr/test_data/test_graph_id1");
	std::cout << "Start to read" <<std::endl;
	UndirectedCompGraph<std::string>* ucgraph1 = new UndirectedCompGraph<std::string>(
			"/Users/mac-97-41/Work/cluster_local_restrictions/src/clr/test_data/test_graph_struct1",
			"/Users/mac-97-41/Work/cluster_local_restrictions/src/clr/test_data/test_graph_id1"
			);
	std::cout << "Reading finished" <<std::endl;
	//Test get node
	int nodeCount = ucgraph1->get_node_count();
	int edgeCount = ucgraph1->get_edge_count();
	for(int i=0;i<nodeCount;i++){
		std::cout << *ucgraph1->get_node(i)<<"  "<<std::endl;
	}

	//get all edges
	std::vector<int*> edges = ucgraph1->get_edges();
	std::cout<< "The number of nodes is "<<nodeCount<<std::endl;
	std::cout << "The number of edges is "<< edgeCount<<std::endl;
	for(int i=0;i<edgeCount;i++){
		std::cout<< edges[i][0]<<"   "<<edges[i][1]<<std::endl;
	}
	std::cout<<"Output complete"<<std::endl;
	delete(ucgraph1);
}

void test_ReadGraph(){
	params_t* params = new params_t();
	/* initialize the params data structure */
	params->ptype         = METIS_PTYPE_KWAY;
	params->objtype       = METIS_OBJTYPE_CUT;
	params->ctype         = METIS_CTYPE_SHEM;
	params->iptype        = -1;
	params->rtype         = -1;

	params->no2hop        = 0;
	params->minconn       = 0;
	params->contig        = 0;

	params->nooutput      = 0;
	params->wgtflag       = 3;

	params->ncuts         = 1;
	params->niter         = 10;

	params->dbglvl        = 0;
	params->balance       = 0;
	params->seed          = -1;
	params->dbglvl        = 0;

	params->tpwgtsfile    = NULL;

	params->filename      = NULL;
	params->nparts        = 2;

	params->ufactor       = -1;

	params->ubvecstr      = NULL;
	params->ubvec         = NULL;

	params->filename = new char[100];
	strcpy(params->filename,"/Users/mac-97-41/Work/cluster_local_restrictions/software/metis/graphs/4elt.graph");
	graph_t* graph = ReadGraph(params);
	std::cout<<"ReadGraph, number of edges in the graph  "<< graph->nedges<<std::endl;

}




