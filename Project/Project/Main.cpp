#include "defines.h"
#include"Graph.h"
#include "TSP.h"
#include "BP.h"
#include "SA.h"
#include "CplexDirected.h"
# include <dirent.h>
# include <sys/types.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
//#include "CplexDirectedCst.h"
#include <iostream>     // std::cout

void experience (){
	string ext = ".vrp";
	DIR* dir = opendir("data/P/");
	struct dirent *entry;

    while(entry = readdir(dir)) {
    	string filename = entry->d_name;
	    size_t found = filename.find(ext);
	    if (found!=string::npos)
	    {
	    	string fullfile = "data/P/" + filename;
	    	cout << fullfile << endl;

	    	Graph G = Graph(fullfile);

			TSP tsp = TSP ();
			vector<vector<int>> l = tsp.localSearch(G,10);
			EvaluateTour e = EvaluateTour();
			float value = e.evaluate(G, l);

			SA sa = SA();
			vector<vector<int>> l2 = sa.simulatedAnnealing(G, e, l, 2000, 1000.0, 0.99, 100.0);
			float value2 = e.evaluate(G, l2);
			vector<vector<int>> l3;
			
			CplexDirected cd;
			float value3 = cd.compute(&G,&l3);

			G.saveSolution(l, "solutions/" + filename+ "_init.txt", value);
			G.saveSolution(l2, "solutions/" + filename + "_SA.txt", value2);
			G.saveSolution(l3, "solutions/" + filename + "_cplex.txt", value3);

			ofstream myfile;
			myfile.open("result.txt", std::ios::app);
		    myfile << filename << " " << G.opt << " " << value << " " << value2 << " " << value3 << "\n";
			myfile.close();
		}

	}
	closedir(dir);
}

int main(int argc, char** argv)
{
	if (argc == 1){
		cout << "No selected file" << endl;
		return 1;
	}
	
	string filename = argv[1];

	Graph G = Graph(filename);
	G.printGraph();
	if (G.computeRelaxedCapacity())
		cout << "Born inf satisfaite" << endl;
	else
		cout << "Born inf non satisfaite" << endl;

	TSP tsp = TSP ();
	vector<vector<int>> l = tsp.localSearch(G,10);
	EvaluateTour e = EvaluateTour();
	float value = e.evaluate(G, l);

	SA sa = SA();
	vector<vector<int>> l2 = sa.simulatedAnnealing(G, e, l, 2000, 1000.0, 0.99, 100.0);
	float value2 = e.evaluate(G, l2);
	vector<vector<int>> l3;

	G.saveSolution(l, "mysolution/init.txt", value);
	G.saveSolution(l2, "mysolution/SA.txt", value2);

	#ifndef COMPETENCE
		CplexDirected cd;
		float value3 = cd.compute(&G,&l3);

		G.saveSolution(l3, "mysolution/cplex.txt", value3);
	
		ofstream myfile;
		myfile.open("mysolution/result.txt", std::ios::app);
	    myfile << filename << " " << G.opt << " " << value << " " << value2 << " " << value3 << "\n";
	    myfile.close();
	#else
	    ofstream myfile;
		myfile.open("mysolution/result.txt", std::ios::app);
	    myfile << filename << " " << G.opt << " " << value << " " << value2 << "\n";
	    myfile.close();
	#endif
	return 0;
}
