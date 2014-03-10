#ifndef CLR_ALGORITHM_H_
#define CLR_ALGORITHM_H_
#include <clr_graph.h>
#include<stack>
#include<string>
#include<iostream>

//In this header file, algorithms in CLR are defined.

//this function calls metis on a ClrGraph and stores the cluster result in the object cluster
template<class T>
UndirectedGraph<T>* clr_metis(UndirectedGraph<T>* graph, Param* p){

}

//This function calls max-flow algorithm in LEMON package
template<class T>
UndirectedSubgraph<T>* clr_maxflow(UndirectedSubgraph<T>* subgraph1,
		UndirectedSubgraph<T>* subgraph2, Param* p){

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
