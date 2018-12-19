#include <iostream>
#include "Graph.h"
#include <vector>
using namespace std;

/**
Class for the resolution of the Bin Packing Problem
**/
class BP
{
private:
	int getIndexOfMax(std::map<int, int> demand, int dimension, std::vector<int> takenIndexes);
	std::vector<int> getDecreasingOrder(std::map<int, int> demand, int dimension);

public:
	BP();
	std::vector<std::vector<int>> firstFitDecreasing(Graph graph);
};