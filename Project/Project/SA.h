#include <iostream>
#include "Graph.h"
#include "EvaluateTour.h"
#include <vector>
using namespace std;

/**
Class for the Simulated Annealing of the TSP
**/
class SA
{
public:
	SA();
	std::vector<std::vector<int>> simulatedAnnealing(Graph graph, EvaluateTour e, std::vector<std::vector<int>> nodes, int kmax, float t, float alpha, float energy_max);
private:
	std::vector<std::vector<int>> getNeighbor(Graph graph, std::vector<std::vector<int>> s, int m);
	float getProbability(float e, float t);
	std::vector<std::vector<int>> swapClient(Graph graph, std::vector<std::vector<int>> s);
	std::vector<std::vector<int>> addTruck(Graph graph, std::vector<std::vector<int>> s, int m);
	std::vector<std::vector<int>> eraseEmptyTruck(Graph graph, std::vector<std::vector<int>> s);
	std::vector<std::vector<int>> clientChange(Graph graph, std::vector<std::vector<int>> s);
	bool isValidTruck(Graph graph, std::vector<int> s);
	double getEnergy(Graph graph, EvaluateTour e, std::vector<std::vector<int>> sn);
};
