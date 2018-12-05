#include"Graph.h"
#include <vector>
using namespace std;

class TSP
{
public:
	TSP();
	vector<int> nearest_neighbor(Graph graph, vector<int> nodes);
	vector<int> nearest_insertion(Graph graph, vector<int> nodes);
};

