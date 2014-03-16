/*
 * main.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: mac-97-41
 */
#include<iostream>
#include<test/test_graph.h>
int main(int argc, char** argv){
	//test_run_metis_clr();
	//test_UndirectedCompGraph();
	/*For test, check the method to get the size of array in c++*/
	std::vector<int> vec;
	vec.push_back(5);
	vec.push_back(1);
	vec.push_back(10);
	vec.push_back(15);
	vec.push_back(18);
	vec.push_back(30);
	vec.push_back(20);
	vec.shrink_to_fit();

	std::cout<<sizeof(vec)/sizeof(int)<<std::endl;
	std::cout<<sizeof(&vec[0])/sizeof(int)<<std::endl;
	std::cout <<vec.size()<<std::endl;

	int a[5];
	a[0] = 1;
	a[1] = 3;
	a[2] = 5;
	a[3] = 7;
	a[4] = 9;
	std::cout<<"array size  "<<sizeof (a)<<std::endl;
	return 1;
}



