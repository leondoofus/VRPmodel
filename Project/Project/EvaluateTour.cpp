#include "defines.h"
#include "EvaluateTour.h"

using namespace std;

EvaluateTour::EvaluateTour()
{
}


float EvaluateTour::evaluate(Graph graph, vector<vector<int>> tours)
{
	float val = 0;
	for (int it = 0; it != tours.size(); ++it)
	{
		val += graph.distance(tours[it][0], tours[it][1]);
		for (int it2 = 1; it2 != tours[it].size() - 1; ++it2){
			val += graph.distance(tours[it][it2], tours[it][it2 + 1]);
			#ifdef COMPETENCE
				val += 3*graph.getReward(it,tours[it][it2 + 1]);
			#endif
		}
	}
	#ifdef WORKINGTIME
		for (int t = 0; t < tours.size(); t++)
		{
			double time = 0.0;
			for (int i = 1; i < tours[t].size(); i++)
			{
				time += graph.distance(tours[t][i-1], tours[t][i]);
			}
			if (time > graph.maxTime)
			{
				val += time - graph.maxTime;
			}
		}
	#endif
	val += 100 * std::max((int)(tours.size() - graph.vehicles), 0);
	//cout << "Tours value : " << val << endl;
	return val;
}

float EvaluateTour::evaluate(Graph graph, vector<int> tour) //local
{
	float val = 0;
	for (int it2 = 0; it2 != tour.size() - 1; ++it2)
		val += graph.distance(tour[it2], tour[it2 + 1]);
	//cout << "Tour value : " << val << endl;
	return val;
}
