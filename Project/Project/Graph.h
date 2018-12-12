#pragma once
#include <iostream>
#include <string>
#include <map>
using namespace std;

class Position
{
public:
	int x;
	int y;
};

class Graph
{
public:
	string name;
	string type;
	string wtype;
	int dimension;
	int capacity;
	std::map<int, Position> coord;
	std::map<int, int> demand;
	int depot;
	int vehicles;

	Graph(string filename);
	void printGraph(void);
	float distance(int n1, int n2);
	bool computeRelaxedCapacity();
};