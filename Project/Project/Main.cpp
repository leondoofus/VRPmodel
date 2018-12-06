#include"Graph.h"
#include "TSP.h"
#include "BP.h"
#include "EvaluateTour.h"
#include <iostream>     // std::cout

int main(void)
{
	Graph G = Graph("data/A/A-n32-k5.vrp");
	//G.printGraph();
	//cout << G.distance(1,5) << endl;

	BP bp = BP();
	vector<vector<int>> l;
	l = bp.firstFitDecreasing(G);


	/*vector<int> l = {2,3,4};*/
	TSP tsp = TSP();;
	/*tsp.nearestNeighbor(G, l);*/
	EvaluateTour e = EvaluateTour();
	e.evaluate(G, tsp.nearestInsertion(G, l));
	cout << "OPT :" << endl;
	e.evaluate(G, { {21, 31, 19, 17, 13, 7, 26},{12, 1, 16, 30}, {27, 24}, {29, 18, 8, 9, 22, 15, 10, 25, 5, 20}, {14, 28, 11, 4, 23, 3, 2, 6} });
	cout << "Cost expected : 784" << endl;
	return 0;
}