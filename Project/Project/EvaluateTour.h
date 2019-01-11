#include "Graph.h"
#include "vector"

class EvaluateTour
{
public:
	EvaluateTour();
	float evaluate(Graph graph, vector<vector<int>> tours);
	float evaluate(Graph graph, vector<int> tour);
};

