#include "Graph.h"
#include "BP.h"
#include <algorithm>
#include <vector>
using namespace std;

BP::BP()
{

}

/* Returns an integer that is the maximum of the demands of the clients
 * that is not already in the decreasing order list takenIndexes
 */
int BP::getIndexOfMax(std::map<int, int> demand, int dimension, std::vector<int> takenIndexes)
{
	/* index of max */
	int indexOfMax = 0;

	/* if takenIndexes is empty, return index of the max */
	if (takenIndexes.empty()) {
		for (int i = 2; i <= dimension; ++i)
		{
			if (demand[i] > demand[indexOfMax])
			{
				indexOfMax = i;
			}
		}
		/* if takenIndexes is not empty, return index of the max that is not in takenIndexes*/
	}
	else {
		for (int i = 2; i <= dimension; ++i)
		{
			if (demand[i] > demand[indexOfMax])
			{
				if (std::find(takenIndexes.begin(), takenIndexes.end(), i) == takenIndexes.end()) {
					indexOfMax = i;
				}
			}
		}
	}
	return indexOfMax;
}

/* Returns a list of integers that is the list of the indexes
 * of clients sorted by decreasing order of their demand
 */
std::vector<int> BP::getDecreasingOrder(std::map<int, int> demand, int dimension)
{
	/* list of the decreasing order of the indexes of clients
	   in function of their demands */
	std::vector<int> takenIndexes;
	int index;

	/* till every clients is in takenIndexes, we add the index of the max demand */
	while (takenIndexes.size() <= dimension - 1)
	{
		index = getIndexOfMax(demand, dimension, takenIndexes);
		takenIndexes.push_back(index);
	}

	return takenIndexes;
}

/* First Fit Decreasing Algorithme */
std::vector<std::vector<int>> BP::firstFitDecreasing(Graph graph)
{
	std::vector<int> decreasingOrder = getDecreasingOrder(graph.demand, graph.dimension);
	std::vector<std::vector<int>> allocation;
	std::vector<int> weights;

	int d;
	int k = 0;
	vector<int> tmp;
	allocation.push_back(tmp);
	weights.push_back(0);

	bool demandAdded;

	/* for every demands i */
	for (int i = 2; i <= graph.dimension; ++i)
	{
		d = graph.demand[i];
		demandAdded = false;
		/* for every truck, we try to put this demand */
		for (int j = 0; j <= k; ++j)
		{
			/* if truck j can take care of demand i then add i to j */
			if (weights[j] + d <= graph.capacity)
			{
				allocation[j].push_back(i);
				weights[j] += d;
				demandAdded = true;
				break;
			}
		}
		/* if demand has not been added to a truck, add a truck and add demand to it */
		if (!demandAdded)
		{
			k += 1;
			vector<int> tmp;
			allocation.push_back(tmp);
			allocation[k].push_back(i);
			weights.push_back(d);
		}
	}
	for (int it = 0; it != allocation.size(); ++it)
	{
		cout << "----------" << endl;
		for (int it2 = 0; it2 != allocation[it].size(); ++it2)
			cout << allocation[it][it2] << endl;
	}
	return allocation;
}