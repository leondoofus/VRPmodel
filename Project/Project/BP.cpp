#include "defines.h"
#include "Graph.h"
#include "BP.h"
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

BP::BP()
{

}

/* Returns a list of integers that is the list of the indexes of clients sorted by decreasing order of their demand
 */
std::vector<int> BP::getDecreasingOrder(std::map<int, int> demand, int dimension)
{
	vector<int> v(1,2);
	for (int i = 3; i <= dimension; i ++)
	{
		int index = 0;
		for (int j = 0;j<v.size();j++){
			if (demand[i] > demand[v[j]]){
				index = j;
				break;
			}
			index = j+1;
		}
		v.insert(v.begin()+index,i);
	}

	return v;
}

/* First Fit Decreasing Algorithm: Returns a list of list that represents the clients affected to each truck
*/
std::vector<std::vector<int>> BP::firstFitDecreasing(Graph graph)
{
	#ifdef COMPETENCE
		return firstFitDecreasingService(graph);
	#endif
	std::vector<int> decreasingOrder = getDecreasingOrder(graph.demand, graph.dimension);
	std::vector<std::vector<int>> allocation;
	std::vector<int> weights;

	int d;
	int index;
	int k = 0;
	vector<int> tmp;
	allocation.push_back(tmp);
	weights.push_back(0);

	bool demandAdded;

	/* for every demands i */
	for (int i = 0; i < decreasingOrder.size(); ++i)
	{
		index = decreasingOrder[i];
		d = graph.demand[index];
		demandAdded = false;
		/* for every truck, we try to put this demand */
		for (int j = 0; j <= k; ++j)
		{
			/* if truck j can take care of demand i then add i to j */
			if (weights[j] + d <= graph.capacity)
			{
				allocation[j].push_back(index);
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
			allocation[k].push_back(index);
			weights.push_back(d);
		}
	}
	// for (int it = 0; it != allocation.size(); ++it)
	// {
	// 	cout << "----------" << endl;
	// 	for (int it2 = 0; it2 != allocation[it].size(); ++it2)
	// 		cout << allocation[it][it2] << endl;
	// }
	return allocation;
}

std::vector<std::vector<int>> BP::firstFitDecreasingService(Graph graph)
{
		std::vector<int> decreasingOrder = getDecreasingOrder(graph.demand, graph.dimension);
		cout << decreasingOrder.size() << endl;
		std::vector<std::vector<int>> allocation;
		std::vector<int> weights;

		int d;
		int index;
		

		bool demandAdded;

		std::vector<int> vab;
		std::vector<int> va;
		std::vector<int> vb;
		std::vector<int> ve;

		for (int i = 0; i < decreasingOrder.size(); ++i)
		{
			if (graph.clientType[decreasingOrder[i]].compare("AB") == 0)
			{
				vab.push_back(decreasingOrder[i]);
				continue;
			}
			if (graph.clientType[decreasingOrder[i]].compare("A") == 0)
			{
				va.push_back(decreasingOrder[i]);
				continue;
			}
			if (graph.clientType[decreasingOrder[i]].compare("B") == 0)
			{
				vb.push_back(decreasingOrder[i]);
				continue;
			}
			if (graph.clientType[decreasingOrder[i]].compare("empty") == 0)
			{
				ve.push_back(decreasingOrder[i]);
				continue;
			}
		}

		cout << vab.size() << endl;
		cout << va.size() << endl;
		cout << vb.size() << endl;
		cout << ve.size() << endl;

		for (int i = 0; i<graph.vehicles; i++){
			vector<int> tmp;
			allocation.push_back(tmp);
			weights.push_back(0);
		}

		int k = allocation.size();

		std::vector<int> rest;

		for (int v=0; v<vab.size();v++)
		{
			bool demandAdded = false;
			for (int i=0; i<graph.vehicleType["AB"].size(); i++)
			{
				if (weights[graph.vehicleType["AB"][i]] + graph.demand[vab[v]] <= graph.capacity)
				{
					allocation[graph.vehicleType["AB"][i]].push_back(vab[v]);
					weights[graph.vehicleType["AB"][i]] += graph.demand[vab[v]];
					demandAdded = true;
					break;
				}
			}
			if (!demandAdded) rest.push_back(vab[v]);
		}
		for (int v=0; v<va.size();v++)
		{
			bool demandAdded = false;
			for (int i=0; i<graph.vehicleType["A"].size(); i++)
			{
				if (weights[graph.vehicleType["A"][i]] + graph.demand[va[v]] <= graph.capacity)
				{
					allocation[graph.vehicleType["A"][i]].push_back(va[v]);
					weights[graph.vehicleType["A"][i]] += graph.demand[va[v]];
					demandAdded = true;
					break;
				}
			}
			if (!demandAdded) rest.push_back(va[v]);
		}
		for (int v=0; v<vb.size();v++)
		{
			bool demandAdded = false;
			for (int i=0; i<graph.vehicleType["B"].size(); i++)
			{
				if (weights[graph.vehicleType["B"][i]] + graph.demand[vb[v]] <= graph.capacity)
				{
					allocation[graph.vehicleType["B"][i]].push_back(vb[v]);
					weights[graph.vehicleType["B"][i]] += graph.demand[vb[v]];
					demandAdded = true;
					break;
				}
			}
			if (!demandAdded) rest.push_back(vb[v]);
		}

		rest.insert(rest.end(),ve.begin(),ve.end());
		

		/* for every demands i */
		for (int i = 0; i < rest.size(); ++i)
		{
			index = rest[i];
			d = graph.demand[index];
			demandAdded = false;
			/* for every truck, we try to put this demand */
			for (int j = 0; j <= k; ++j)
			{
				/* if truck j can take care of demand i then add i to j */
				if (weights[j] + d <= graph.capacity)
				{
					allocation[j].push_back(index);
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
				allocation[k].push_back(index);
				weights.push_back(d);
			}
		}
		// for (int it = 0; it != allocation.size(); ++it)
		// {
		// 	cout << "----------" << endl;
		// 	for (int it2 = 0; it2 != allocation[it].size(); ++it2)
		// 		cout << allocation[it][it2] << " ";
		// 	cout << endl;
		// }
		return allocation;
}

