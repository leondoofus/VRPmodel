#include"Graph.h"
#include <vector>
using namespace std;

class TSP
{
public:
	TSP();
	vector<vector<int>> nearestNeighbor(Graph graph, vector<vector<int>> nodes);
	vector<vector<int>> nearestInsertion(Graph graph, vector<vector<int>> nodes);
	vector<vector<int>> localSearch (Graph graph, int tabuSize);
private:
	vector<int> nearest_neighbor(Graph graph, vector<int> nodes);
	vector<int> nearest_insertion(Graph graph, vector<int> nodes);
	vector<int> swap (int pos1, int pos2, vector<int> vect);
	bool fitnessCondition (vector<float> fitness, int tabuSize);
	bool isEqual(vector<int> v1, vector<int> v2);
	vector<int> getBestNeighbour (Graph graph, vector<int> solution, int tabuSize);
	vector<vector<int>> bestNeighbourSearch (Graph graph, vector<vector<int>> solution, int tabuSize);
;};

