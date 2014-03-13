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
/*In this header, tests of the classes and function sin clr_graph.h are written. The detailed source codes are implemented in
 * source/test/test_graph.cpp */

/*1.Test the constructors for UndirectedCompGraph*/
void test_UndirectedCompGraph(){
	UndirectedCompGraph<std::string> ucgraph1("/Users/mac-97-41/Work/cluster_local_restrictions/test/test_graph_struct1",
			"/Users/mac-97-41/Work/cluster_local_restrictions/test/test_graph_id1");

}


#endif /* TEST_GRAPH_H_ */
