#include "SA.h"
#include "BP.h"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

SA::SA()
{
}

/**
**/
std::vector<std::vector<int>> SA::simulatedAnnealing(Graph graph, EvaluateTour e, std::vector<std::vector<int>> nodes, int kmax, float t, float alpha, float energy_max)
{
	std::vector<std::vector<int>> s = nodes;
	std::vector<std::vector<int>> g = nodes;

	float energy_s = e.evaluate(graph, nodes) + 100 * nodes.size();
	float energy_g = energy_s;

	std::vector<std::vector<int>> sn;
	float energy_sn;

	float temperature = t;

	int m = nodes.size() + 1;

	srand(time(NULL));
	int k = 0;
	while(k < kmax && energy_s > energy_max)
	{
		sn = getNeighbor(graph, s, m);
		energy_sn = e.evaluate(graph, sn) + 100 * sn.size();

		if(energy_sn < energy_s || rand() < getProbability(energy_sn-energy_s, temperature))
		{
			s = sn;
			energy_s = energy_sn;
		}

		if(energy_s < energy_g)
		{
			g = s;
			energy_g = energy_s;
		}

		k += 1;
		temperature *= alpha;
	}

	return g;
}

float SA::getProbability(float e, float t)
{
	return exp(-e / t);
}

std::vector<std::vector<int>> SA::getNeighbor(Graph graph, std::vector<std::vector<int>> s, int m)
{
	srand(time(NULL));

	int r = rand();

	if(r < 1/3.0)
	{
		return clientChange(graph, s);
	}

	if (r < 2/3.0)
	{
		return swapTruck(graph, s);
	}

	if (r < 5/6.0)
	{
		return addTruck(graph, s, m);
	}

	if (r < 1.0)
	{
		return eraseEmptyTruck(graph, s);
	}

	return s;
}

std::vector<std::vector<int>> SA::swapTruck(Graph graph, std::vector<std::vector<int>> s)
{
	int i;
	int j1;
	int j2;
	int tmp;

	srand(time(NULL));
	do
	{
		i = rand() * s.size();
	} while (s[i].size() < 4);

	if (s[i].size() == 4)
	{
		tmp = s[i][1];
		s[i][1] = s[i][2];
		s[i][2] = tmp;

		return s;
	}

	j1 = rand() * (s[i].size() - 1) + 1;;
	do
	{
		j2 = rand() * (s[i].size() - 1) + 1;;
	} while (j2 == j1);


	tmp = s[i][j1];
	s[i][j1] = s[i][j2];
	s[i][j2] = tmp;

	return s;
}

std::vector<std::vector<int>> SA::addTruck(Graph graph, std::vector<std::vector<int>> s, int m)
{
	if (s.size() == m)
	{
		return s;
	}
	std::vector<int> v;
	v.push_back(1);
	v.push_back(1);
	s.insert(s.begin(), v);

	return s;
}

std::vector<std::vector<int>> SA::eraseEmptyTruck(Graph graph, std::vector<std::vector<int>> s)
{
	for (int k = 0; k < s.size(); ++k)
	{
		if (s[k].size() == 2)
		{
			s.erase(s.begin()+k); // this line could ruin the loop
			return s;
		}
	}

	return s;
}

std::vector<std::vector<int>> SA::clientChange(Graph graph, std::vector<std::vector<int>> s)
{
	std::vector<std::vector<int>> sn = s;
	bool neighborNotFound = true;
	int i1;
	int i2;
	int j1;
	int j2;
	std::vector<int> s1;
	std::vector<int> s2;

	while (neighborNotFound)
	{
		srand(time(NULL));
		i1 = rand() * s.size();
		do
		{
			i2 = rand() * s.size();
		} while (i2 == i1);
		s1 = s[i1];
		s2 = s[i2];

		//j1 = rand() * (s1.size()-1) + 1;
		//j2 = rand() * (s2.size()-1) + 1;

		// You mean this ?
		j1 = rand() % (s1.size() - 1) + 1;
		j2 = rand() % (s2.size() - 1) + 1;

		
		//s1.insert(j1, s2[j2]);
		//s2.erase(j2);
		s1.insert(s1.begin() + j1, s2[j2]);
		s2.erase(s2.begin() + j2);

		if (isValidTruck(graph, s1))
		{
			neighborNotFound = false;
		}

	}

	sn[i1] = s1;
	sn[i2] = s2;

	return sn;
}

bool SA::isValidTruck(Graph graph, std::vector<int> s)
{
	int sumD = 0;

	for (int k = 0; k < s.size(); ++k)
	{
		sumD += graph.demand[s[k]];
		
		if (sumD > graph.capacity)
		{
			return false;
		}
	}

	return true;
}