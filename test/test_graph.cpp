
#include<test/test_graph.h>
#include<metis_function.h>

/*****************************************************************
 * In this test file package, we test the following methods in Undi-
 * rectedCompGraph:
 * 1. Constructor
 * 2. get_node_count
 * 3. get_edge_count
 * 4. add_node
 * 5. get_edge_weight * 2
 * 6. add_edge
 * 7. get_graph_t
 * 8. get_neighbours
 * 9. get_conn_subgraphs
 * 10. is_edge, is_conn
 * 11. search_node
 *****************************************************************/

class TestUndirectedCompGraph : public ::testing::Test{
	//Implement the virtual methods
	std::string structInput;
	std::string idInput;
	UndirectedCompGraph<std::string>* graph;

protected:
	/*****************************************************************
	 * SetUp() function inits
	 *****************************************************************/
	virtual void SetUp(){
		structInput = "/Users/mac-97-41/Work/cluster_local_restrictions/src/clr/test_data/test_graph_struct1";
		idInput ="/Users/mac-97-41/Work/cluster_local_restrictions/src/clr/test_data/test_graph_id1";
		std::cout<< "Graph structure input:  "<< structInput <<std::endl;
		std::cout<< "Id input:   "<< idInput<<std::endl;
	}

	virtual void TearDown(){
		delete(graph);
	}
public:
	virtual ~TestUndirectedCompGraph(){

	}
	TestUndirectedCompGraph(){
		graph = new UndirectedCompGraph<std::string>(structInput, idInput);
	}
};

/*****************************************************************
 *
 *****************************************************************/
TEST_F(TestUndirectedCompGraph, test_constructor){

}

/*****************************************************************
 *
 *****************************************************************/
TEST_F(TestUndirectedCompGraph, test_get_count){

}


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
	//
	//Get all edges
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

}




