#include <iostream>
#include "Graph.h"
#include <vector>
using namespace std;

class BinPacking
{
private:
	int getIndexOfMax(std::map<int, int> demand, int dimension, std::vector<int> takenIndexes);
	std::vector<int> getDecreasingOrder(std::map<int, int> demand);

public:
	Graph graph;

	BinPacking(Graph newGraph);
	std::vector<std::vector<int>> firstFitDecreasing(void);
};