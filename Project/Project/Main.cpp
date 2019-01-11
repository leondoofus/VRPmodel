#include"Graph.h"
#include "TSP.h"
#include "BP.h"
#include "SA.h"
//#include "EvaluateTour.h"
#include "MTZ.h"
#include <iostream>     // std::cout

int main(void)
{
	Graph G = Graph("data/A/A-n39-k6.vrp");
	G.printGraph();
	if (G.computeRelaxedCapacity())
		cout << "Born inf satisfaite" << endl;
	else
		cout << "Born inf non satisfaite" << endl;
	//cout << G.distance(1,5) << endl;

	//BP bp = BP();
	vector<vector<int>> l;
	//l = bp.firstFitDecreasing(G);


	/*vector<int> l = {2,3,4};*/
	TSP tsp = TSP();;
	//l = tsp.nearestInsertion(G, l);
	/*tsp.nearestNeighbor(G, l);*/
	EvaluateTour e = EvaluateTour();
	//cout << "Tours value : " << e.evaluate(G, l) << endl;
	
	e.evaluate(G,{1,30,31,32,1});
	e.evaluate(G,{1,30,32,31,1});
	e.evaluate(G,{1,31,30,32,1});
	cout << "---------" << endl;
	tsp.localSearch(G, 5);

	/*SA sa = SA();
	l = sa.simulatedAnnealing(G, e, l, 2000, 1000.0, 0.99, 100.0);
	cout << e.evaluate(G, l) << endl;
	for (int it = 0; it != l.size(); ++it)
		{
			cout << "----------" << endl;
			for (int it2 = 0; it2 != l[it].size(); ++it2)
				cout << l[it][it2] << " ";
			cout << endl;
		}*/

	cout << "----------" << endl;
	//MTZ mtz;
	//mtz.compute(&G);
	return 0;
}
