#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
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
	double maxTime;
	int opt;
	std::map<int, string> clientType;
	std::map<string, vector<int>> vehicleType;

	Graph(string filename);
	void printGraph(void);
	float distance(int n1, int n2);
	bool computeRelaxedCapacity();
	void saveSolution(std::vector<std::vector<int>> sol, string filename);
	float getReward(int tour, int client);
private:
	void initClientType();
	void initVehicleType();

};
