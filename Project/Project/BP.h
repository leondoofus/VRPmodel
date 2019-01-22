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
	std::vector<int> getDecreasingOrder(std::map<int, int> demand, int dimension);

public:
	BP();
	std::vector<std::vector<int>> firstFitDecreasing(Graph graph);
	std::vector<std::vector<int>> firstFitDecreasingService(Graph graph);
};