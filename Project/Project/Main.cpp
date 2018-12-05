#include"Graph.h"
#include "TSP.h"
#include "BP.h"
#include <iostream>     // std::cout

int main(void)
{
	Graph G = Graph("data/A/A-n32-k5.vrp");
	//G.printGraph();
	//cout << G.distance(1,5) << endl;

	BP bp = BP();
	vector<vector<int>> l;
	l = bp.firstFitDecreasing(G);
	cout << "wooooooooooo" << endl;


	/*vector<int> l = {2,3,4};*/
	TSP tsp = TSP();;
	/*tsp.nearestNeighbor(G, l);*/
	cout << "-----------" << endl;
	tsp.nearestInsertion(G, l);
	return 0;
}