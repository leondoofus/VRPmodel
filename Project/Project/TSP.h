#include"Graph.h"
#include <vector>
using namespace std;

class TSP
{
public:
	TSP();
	vector<vector<int>> nearestNeighbor(Graph graph, vector<vector<int>> nodes);
	vector<vector<int>> nearestInsertion(Graph graph, vector<vector<int>> nodes);
	vector<int> nearest_neighbor(Graph graph, vector<int> nodes);
	vector<int> nearest_insertion(Graph graph, vector<int> nodes);
	vector<vector<int>> localSearch (Graph graph, int tabuSize);
private:
	vector<vector<int>> bestNeighborSearch (Graph graph, vector<vector<vector<int>>>* tabu)
;};

