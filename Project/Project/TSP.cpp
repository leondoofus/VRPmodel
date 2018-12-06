#include "TSP.h"

TSP::TSP()
{
}


vector<int> TSP::nearest_neighbor(Graph graph, vector<int> nodes)
{
	vector<int> tour;
	tour.push_back(graph.depot);
	while (nodes.size() > 0)
	{
		int index = 0;
		float vmin = graph.distance(tour[tour.size()-1], nodes[index]);
		for (int it = 0; it != nodes.size(); ++it)
		{
			float v = graph.distance(tour[tour.size() - 1], nodes[it]);
			if  (v < vmin)
			{
				vmin = v;
				index = it;
			}
		}
		tour.push_back(nodes[index]);
		nodes.erase(nodes.begin() + index);
	}
	tour.push_back(graph.depot);
	/*for (int it = 0; it != tour.size(); ++it)
		cout << tour[it] << endl;*/
	return tour;
}

vector<int> TSP::nearest_insertion(Graph graph, vector<int> nodes)
{
	vector<int> tour;
	tour.push_back(graph.depot);
	tour.push_back(graph.depot);
	while (nodes.size() > 0)
	{
		int node = 0;
		float deltaF = graph.distance(tour[0], nodes[node]) + graph.distance(nodes[node], tour[1]) - graph.distance(tour[0], tour[1]);
		int index = 0;
		for (int it = 0; it != nodes.size(); ++it)
		{
			for (int it2 = 0; it2 != tour.size() - 1; ++it2)
			{
				float deltaF2 = graph.distance(tour[it2], nodes[it]) + graph.distance(nodes[it], tour[it2 + 1]) - graph.distance(tour[it2], tour[it2 +1]);
				if (deltaF2 < deltaF)
				{
					node = it;
					index = it2;
				}
			}
		}
		tour.insert(tour.begin() + index + 1, nodes[node]);
		nodes.erase(nodes.begin() + node);
	}
	/*for (int it = 0; it != tour.size(); ++it)
		cout << tour[it] << endl;*/
	return tour;
}

vector<vector<int>> TSP::nearestNeighbor(Graph graph, vector<vector<int>> nodes)
{
	vector<vector<int>> tours;

	for (int i = 0; i != nodes.size(); ++i)
	{
		tours.push_back(nearest_neighbor(graph, nodes[i]));
	}

	for (int it = 0; it != tours.size(); ++it)
	{
		cout << "----------" << endl;
		for (int it2 = 0; it2 != tours[it].size(); ++it2)
			cout << tours[it][it2] << " ";
		cout << endl;
	}
	return tours;
}

vector<vector<int>> TSP::nearestInsertion(Graph graph, vector<vector<int>> nodes)
{
	vector<vector<int>> tours;

	for (int i = 0; i != nodes.size(); ++i)
	{
		tours.push_back(nearest_insertion(graph, nodes[i]));
	}

	for (int it = 0; it != tours.size(); ++it)
	{
		cout << "----------" << endl;
		for (int it2 = 0; it2 != tours[it].size(); ++it2)
			cout << tours[it][it2] << " ";
		cout << endl;
	}
	return tours;
}
