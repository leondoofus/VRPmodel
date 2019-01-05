#include"Graph.h"
#include "TSP.h"
#include "BP.h"
#include "SA.h"
//#include "EvaluateTour.h"
//#include "MTZ.h"
#include <iostream>     // std::cout

int main(void)
{
	Graph G = Graph("data/A/A-n32-k5.vrp");
	G.printGraph();
	if (G.computeRelaxedCapacity())
		cout << "Born inf satisfaite" << endl;
	else
		cout << "Born inf non satisfaite" << endl;
	//cout << G.distance(1,5) << endl;

	BP bp = BP();
	vector<vector<int>> l;
	l = bp.firstFitDecreasing(G);


	/*vector<int> l = {2,3,4};*/
	TSP tsp = TSP();;
	l = tsp.nearestInsertion(G, l);
	/*tsp.nearestNeighbor(G, l);*/
	EvaluateTour e = EvaluateTour();
	cout << "Tour value : " << e.evaluate(G, l) << endl;
	cout << "OPT :" << endl;
	e.evaluate(G, { {1, 22, 32, 20, 18, 14, 8, 27, 1},{1, 13, 2, 17, 31, 1}, {1, 28, 25, 1}, {1, 30, 19, 9, 10, 23, 16, 11, 26, 6, 21, 1}, {1, 15, 29, 12, 5, 24, 4, 3, 7, 1} });
	cout << "Cost expected : 784" << endl;

	SA sa = SA();
	l = sa.simulatedAnnealing(G, e, l, 2000, 1000.0, 0.99, 0.0);
	cout << e.evaluate(G, l) << endl;
	cout << "----------" << endl;
	//MTZ mtz;
	//mtz.compute(&G);
	return 0;
}