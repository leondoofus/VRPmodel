#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <regex>


Graph::Graph(string filename)
{
	cout << "Constructing graph from " << filename << "\n";
	ifstream myfile;
	myfile.open(filename.c_str());
	if (myfile)
	{
		string line;
		while (getline(myfile, line))
		{
			std::size_t found = line.find("NAME");
			if (found != string::npos)
				name = line.substr(7);
			found = line.find("VEHICLES");
			if(found != string::npos){
	            std::regex r ("[[:digit:]]+");
	            std::smatch s;
	            string cut = line.substr(found);
	            std::regex_search(cut,s,r);
	            vehicles = stoi(s[0]);
	        }
	        found = line.find("trucks");
			if(found != string::npos){
	            std::regex r ("[[:digit:]]+");
	            std::smatch s;
	            string cut = line.substr(found);
	            std::regex_search(cut,s,r);
	            vehicles = stoi(s[0]);
	        }
	        found = line.find("Optimal");
			if(found != string::npos){
	            std::regex r ("[[:digit:]]+");
	            std::smatch s;
	            string cut = line.substr(found);
	            std::regex_search(cut,s,r);
	            opt = stoi(s[0]);
	        }
			found = line.find("TYPE");
			if (found != string::npos && type.empty())
				type = line.substr(7);
			found = line.find("EDGE_WEIGHT_TYPE");
			if (found != string::npos)
				wtype = line.substr(19);
			found = line.find("DIMENSION");
			if (found != string::npos){
				std::regex r ("[[:digit:]]+");
	            std::smatch s;
	            string cut = line.substr(found);
	            std::regex_search(cut,s,r);
	            dimension = stoi(s[0]);
	        }
			found = line.find("CAPACITY");
			if (found != string::npos){
				std::regex r ("[[:digit:]]+");
	            std::smatch s;
	            string cut = line.substr(found);
	            std::regex_search(cut,s,r);
	            capacity = stoi(s[0]);
	        }
			found = line.find("NODE_COORD_SECTION");
			if (found != string::npos)
				break;
		}
		string tmp[3];
		while (getline(myfile, line))
		{
			std::size_t found = line.find("DEMAND_SECTION");
			if (found != string::npos)
				break;
			/*
			while (line[0] == '\œ ')
			{
				line.erase(0, 1);
			}*/
			std::stringstream lineStream(line);
			int i = 0;
			while (!lineStream.eof() && i < 3) {
                lineStream >> tmp[i];
                i++;
            }

			int key = stoi(tmp[0]);
			line.erase(0, line.find('\ ') + 1);
			int x = stoi(tmp[1]);
			line.erase(0, line.find('\ ') + 1);
			int y = stoi(tmp[2]);
			Position obj;
			obj.x = x;
			obj.y = y;
			coord[key] = obj;
		}
		tmp[2];
		while (getline(myfile, line))
		{
			std::size_t found = line.find("DEPOT_SECTION");
			if (found != string::npos)
				break;
			/*
			while (line[0] == '\ ')
			{
				line.erase(0, 1);
			}*/
			std::stringstream lineStream(line);
			int i = 0;
			while (!lineStream.eof() && i < 2) {
                lineStream >> tmp[i];
                i++;
            }
			int key = stoi(tmp[0]);
			int value = stoi(tmp[1]);
			demand[key] = value;
		}
		getline(myfile, line);
		while (line[0] == '\ ')
		{
			line.erase(0, 1);
		}
		depot = stoi(line);
		myfile.close();

		maxTime = 0;
		for (int i=1; i <= dimension; i++)
		{
			maxTime += distance(depot, i);
		}
		maxTime *= 1.5 / vehicles;
	}
	else
	{
		cerr << "File error.\n";
	}
}


void Graph::printGraph(void)
{
	cout << "Name : " << name << endl;
	cout << "Type : " << type << endl;
	cout << "Weight type : " << wtype << endl;
	cout << "Vehicles : " << vehicles << endl;
	cout << "Dimension : " << dimension << endl;
	cout << "Capacity : " << capacity << endl;
	cout << "Node coord :" << endl;
	// for (int i = 1; i <= dimension; i++)
	// 	cout << i << " " << coord[i].x << " " << coord[i].y << endl;
	cout << "Demand : " << endl;
	int sum = 0;
	// for (int i = 1; i <= dimension; i++){
	// 	cout << i << " " << demand[i] << endl;
	// 	sum += demand[i];
	// }
	cout << "Total : " << sum << endl;
	cout << "Depot : " << depot << endl;
	cout << "OPT : " << opt << endl;
}

float Graph::distance(int node1, int node2)
{
	return sqrt((float)(pow(coord[node1].x - coord[node2].x,2) + pow(coord[node1].y - coord[node2].y,2)));
}

bool Graph::computeRelaxedCapacity()
{
	int sum = 0;
	for (int i = 1; i <= dimension; i++){
		sum += demand[i];
	}
	return sum <= capacity * vehicles;
}

void Graph::saveSolution(std::vector<std::vector<int>> sol, string filename)
{
	cout << "oki";
	ofstream myfile;
	myfile.open(filename);
	myfile << "COORD\n";
	for (int i = 1; i <= dimension; i++)
	{
		myfile << i << " " << coord[i].x << " " << coord[i].y << "\n";
	}

	myfile << "TRUCKS\n";
	for (int i = 0; i < sol.size(); i++)
	{
		myfile << i << " ";
		for (int j = 1; j < sol[i].size() - 1; j++)
		{
			myfile << sol[i][j] << " ";
		}
		myfile << "\n";
	}

	myfile.close();
}
