#ifndef CLR_ALGORITHM_H_
#define CLR_ALGORITHM_H_
#include <clr_graph.h>
#include<stack>
#include<string>
#include<iostream>
#include<./lemon/preflow.h>
#include<clr_param.h>

//In this header file, algorithms in CLR are defined.

//this function calls metis on a ClrGraph and stores the cluster result in the object cluster
template<class T>
UndirectedGraph<T>* clr_metis(UndirectedGraph<T>* graph, Param* p){

}

//This function calls max-flow algorithm in LEMON package
template<class T>
UndirectedSubgraph<T>* clr_maxflow(UndirectedSubgraph<T>* subgraph1,
		UndirectedSubgraph<T>* subgraph2, Param* p){

	//Create the directed graph
	DirectedLemonGraph<T> directedGraph(subgraph1, subgraph2);
	//Run maxflow algorithm
	lemon::Preflow preflow(directedGraph,directedGraph.get_capacity_map(),
			directedGraph.get_source_node(), directedGraph.get_sink_node());
	//Return two subgraphs
	preflow.run();
	std::vector<lemon::ListDigraph::Node> lemonNodes = directedGraph.get_lemon_nodes();
	std::vector<int> nodes = directedGraph.get_nodes();

	std::vector<int> nodesSubgraph1;
	std::vector<int> nodesSubgraph2;

	//Add nodes to nodesSubgraph1, nodesSubgraph2
	for(int i=0;i<lemonNodes.size();i++){
		if(preflow.minCut(lemonNodes[i]))
			nodesSubgraph1.push_back(i);
		else nodesSubgraph2.push_back(i);
	}


	UndirectedSubgraph<T> refinedSubgraphs[2];
	if(p->graphType == MATRIX_GRAPH){
		//Use matrix to init
		//refinedSubgraph1 = new UndirectedMatrixSubgraph
	}else if(p->graphType == CSR_GRAPH){
		&refinedSubgraphs[0] = new UndirectedCompSubgraph(&nodesSubgraph1[0], subgraph1->get_super_graph());
		&refinedSubgraphs[1] = new UndirectedCompSubgraph(&nodesSubgraph2[0], subgraph2->get_super_graph());
	}

	return refinedSubgraphs;
}



//This is the main function to run clr
template<class T>
Cluster<T>* clr_run(UndirectedGraph<T>* graph, Param* p){
	//The algorithm runs in a recursive manner
	std::stack<Graph<T>*> graphStack;
	Cluster<T>* clusterRes = new Cluster<T>();
	graphStack.push_back(graph);
	while(! graphStack.empty()){
		UndirectedGraph<T>* inputGraph = graphStack.top();
		graphStack.pop();
		double bestScore = -RAND_MAX;
		UndirectedSubgraph<T>* bestSubgraph1, bestSubgraph2;

		for(int i=0;i<p->metisRounds;i++){
			UndirectedSubgraph<T>* subgraph1, subgraph2;
			subgraph1 = clr_metis(inputGraph, p);
			subgraph2 = subgraph1+1;

			for(int j=0;j<p->maxflowRounds;j++){
				subgraph1 = clr_maxflow(subgraph1, subgraph2, p);
				subgraph2 = subgraph1+1;
			}
			double currentScore  =get_partition_score(subgraph1, subgraph2);
			if(bestScore < currentScore){
				bestSubgraph1 = subgraph1;
				bestSubgraph2 = subgraph2;
				bestScore = currentScore;
			}
		}

		if(bestSubgraph1->is_clr_graph())
			clusterRes->add_subgraph(bestSubgraph1);
		else
			graphStack.push(bestSubgraph1);

		if(bestSubgraph2 -> is_clr_graph())
			clusterRes->add_subgraph(bestSubgraph2);
		else
			graphStack.push(bestSubgraph2);
	}

	return clusterRes;
}

#endif
