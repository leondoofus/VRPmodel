#include "EvaluateTour.h"


EvaluateTour::EvaluateTour()
{
}


float EvaluateTour::evaluate(Graph graph, vector<vector<int>> tours)
{
	float val = 0;
	for (int it = 0; it != tours.size(); ++it)
	{
		for (int it2 = 0; it2 != tours[it].size() - 1; ++it2)
			val += graph.distance(tours[it][it2], tours[it][it2 + 1]);
	}
	//cout << "Tours value : " << val << endl;
	return val;
}

float EvaluateTour::evaluate(Graph graph, vector<int> tour)
{
	float val = 0;
	for (int it2 = 0; it2 != tour.size() - 1; ++it2)
		val += graph.distance(tour[it2], tour[it2 + 1]);
	//cout << "Tour value : " << val << endl;
	return val;
}

