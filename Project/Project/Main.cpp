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

int main(void)
{
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
			G.saveSolution(l3, "solutions/" + filename + "_init.txt", value3);

			ofstream myfile;
			myfile.open("Graph.txt");
		    myfile << filename << " " << G.opt << " " << value << " " << value2 << " " << value3 << "\n";
			myfile.close();
		}

	}
	closedir(dir);
	

	// Graph G = Graph("data/P/P-n16-k8.vrp");
	// G.printGraph();
	// if (G.computeRelaxedCapacity())
	// 	cout << "Born inf satisfaite" << endl;
	// else
	// 	cout << "Born inf non satisfaite" << endl;

	// TSP tsp = TSP ();
	// vector<vector<int>> l = tsp.localSearch(G,5);

	// EvaluateTour e = EvaluateTour();
	// float value = e.evaluate(G, l);
	// cout << "Tour value : " << value << endl;

	// SA sa = SA();
	// l = sa.simulatedAnnealing(G, e, l, 2000, 1000.0, 0.99, 100.0);
	// value = e.evaluate(G, l);
	// cout << value << endl;
	// G.saveSolution(l, "test.txt", value);
	// for (int it = 0; it != l.size(); ++it)
	// 	{
	// 		cout << "----------" << endl;
	// 		for (int it2 = 0; it2 != l[it].size(); ++it2)
	// 			cout << l[it][it2] << " ";
	// 		cout << endl;
	// 	}
	// cout << "----------" << endl;
	// CplexDirected cd;
	// cd.compute(&G);
	return 0;
}
