#include"Graph.h"
#include <iostream>     // std::cout

int main(void)
{
	Graph G = Graph("data/A/A-n32-k5.vrp");
	G.printGraph();
	cout << G.distance(1,5) << endl;
	return 0;
}