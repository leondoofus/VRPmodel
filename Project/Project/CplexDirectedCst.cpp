#include "defines.h"
#include "CplexDirectedCst.h"
#include <ilcplex/ilocplex.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>


#define epsilon 0.001

using namespace std;


int getNeighbour (vector<int> solx, vector<int> cycle)
{
	int index = distance(solx.begin(),find(solx.begin(), solx.end(), 1));
	if (find(cycle.begin(), cycle.end(), index) != cycle.end())
	{
		return -1;
	}
	return index;
}

// vector<int> getCycleFromIndex (vector<vector<int>> solx, int index)
// {
// 	// cout << "Getting Cycle From " << index << " : ";
// 	vector<int> cycle;
// 	cycle.push_back(index);
// 	int neighbour = getNeighbour(solx.at(cycle.back()), cycle);
// 	while (neighbour != -1)
// 	{
// 		cycle.push_back(neighbour);
// 		neighbour = getNeighbour(solx.at(cycle.back()), cycle);
// 	}
// 	#ifdef OUTPUT
// 	cout << "Getting Cycle From " << index << " : ";
// 	for (int i=0; i<cycle.size(); i++) cout << cycle.at(i) << " ";
// 	cout << endl;
// 	#endif
// 	return cycle;
// }

// vector<int> getGoodCycleFromIndex (vector<vector<int>> solx, int index)
// {
// 	vector<int> cycle;
// 	cycle.push_back(0);
// 	cycle.push_back(index);
// 	int neighbour = getNeighbour(solx.at(cycle.back()), cycle);
// 	while (neighbour != -1)
// 	{
// 		cycle.push_back(neighbour);
// 		neighbour = getNeighbour(solx.at(cycle.back()), cycle);
// 	}
// 	#ifdef OUTPUT
// 	//cout << "Getting Cycle From " << index << " : ";
// 	for (int i=0; i<cycle.size(); i++) cout << cycle.at(i) << " ";
// 	cout << endl;
// 	#endif
// 	return cycle;
// }

bool getCycles (Graph* G, vector<vector<vector<int>>>* solx, vector<vector<int>>* bad_cycle, vector<vector<int>>* good_cycle)
{
	bool detect = false;
	int i,j;

	vector<int> checked;

	for (i=1;i<G->dimension;i++){
		if (solx->at(0).at(i) == 1)
		{
			if(find(checked.begin(), checked.end(), i) == checked.end()) // i not checked
			{
				vector<int> c = getGoodCycleFromIndex(*solx, i);
				good_cycle->push_back(c);
				checked.insert(checked.end(),c.begin()+1,c.end());
			}
		}
	}
	for (i=1;i<G->dimension;i++)
	{
		if(find(checked.begin(), checked.end(), i) == checked.end()) // i not checked
		{
			vector<int> c = getCycleFromIndex(*solx, i);
			bad_cycle->push_back(c);
			detect = true;
			checked.insert(checked.end(),c.begin(),c.end());
		}
	}
	return detect;
}

ILOLAZYCONSTRAINTCALLBACK2(LazyWeightCutSeparationCst,
			   Graph &, G,			 
			   vector<vector<vector<IloNumVar>	>>&,x
		    ){
  #ifdef OUTPUT
  cout<<"*********** Lazy separation Callback *************"<<endl;
  #endif

	int i,j,k;
	int Q = G.capacity;
	vector<vector<vector<int>>> solx;

	solx.resize(G.vehicles);
	for (k = 0; k<G.vehicles ;k++){
		solx[k].resize(G.dimension);
		for (i = 0; i < G.dimension; i++)
		{
			solx[k][i].resize(G.dimension);
		}
	}


	for (k = 0; k<G.vehicles ;k++)
		for (i = 0; i < G.dimension; i++)
			for (j = 0; j < G.dimension; j++){
				if (getValue(x[k][i][j]) < epsilon)
					solx[k][i][j] = 0;
				else 
					solx[k][i][j] = 1;
			}

	#ifdef OUTPUT
		for (k = 0; k<G.vehicles ;k++){
			for (i = 0; i < G.dimension; i++){
				for (j = 0; j < G.dimension; j++){
					cout << solx[k][i][j] << " ";
				}
				cout << endl;
			}
			cout << "------ fin " << k << endl;
		}
  	#endif
	

  // vector<vector<int>> bad_cycle;
  // vector<vector<int>> good_cycle;

  // if (getCycles(&G, &solx, &bad_cycle, &good_cycle))
  // {
  // 	#ifdef OUTPUT
  // 	cout << "CYCLE(S) OUTSIDE DETECTED, ADDING " << bad_cycle.size() << " CONSTRAINTS" << endl;
  // 	#endif
  // 	for (i=0;i<bad_cycle.size();i++)
  // 	{
  // 		double weight = 0.0;
  // 		vector<int> c = bad_cycle.at(i);
  // 		IloExpr cviolated(getEnv());
  // 		for (j = 0; j < c.size(); j++) {
  // 			weight += (double)G.demand[c.at(j)+1];
		// 	for (int k = 0; k < G.dimension; k++) {
		// 		if (find(c.begin(), c.end(), k) == c.end())
		// 			cviolated += x[c.at(j)][k];
		// 	}
		// }
		// int W = (int)ceil(weight/(double)Q);
		// IloRange ViolatedCst = IloRange(cviolated >= W);
		// add(ViolatedCst,IloCplex::UseCutPurge);
  // 	}
  // }else{
  // 	#ifdef OUTPUT
  // 	cout << "NO CYCLE OUTSIDE DETECTED" << endl;
  // 	#endif
  // }

  // #ifdef OUTPUT
  // cout << "CHECKING " << good_cycle.size() << " GOOD CYCLE(S) ... " << endl;
  // #endif

  // for (i=0;i<good_cycle.size();i++)
  // 	{
  // 		double weight = 0.0;
  // 		vector<int> c = good_cycle.at(i);
  // 		IloExpr cviolated(getEnv());
  // 		for (j = 1; j < c.size(); j++) {
  // 			weight += (double)G.demand[c.at(j)+1];
		// 	for (int k = 0; k < G.dimension; k++) {
		// 		if (k == 0) cviolated += x[c.at(j)][k];
		// 		else {
		// 			if (find(c.begin(), c.end(), k) == c.end())
		// 				cviolated += x[c.at(j)][k];
		// 		}
		// 	}
		// }
		// if (weight > (double)Q) {
		// 	int W = (int)ceil(weight/(double)Q);
		// 	IloRange ViolatedCst = IloRange(cviolated >= W);
		// 	add(ViolatedCst,IloCplex::UseCutPurge);
		// 	#ifdef OUTPUT
		//   	cout << "Weight = " << weight <<" ADDING 1 CONSTRAINT" << endl;
		//   	#endif
		// }
  // 	}

}








void CplexDirectedCst::compute(Graph *graph) { //undirected graph
	string name, nameext, nameextsol;
	int i, j, k;

	int depot = graph->depot - 1; //Reformule with an offset of 1
	int Q = graph->capacity;

	vector<int> sol;


	//////////////
	//////  DATA
	//////////////

	name = graph->name;
	//nameext = name + ".gra";
	//nameextsol = name + ".acycl";

	//////////////
	//////  CPLEX INITIALIZATION
	//////////////


	IloEnv   env;
	IloModel model(env);


	////////////////////////
	//////  VAR
	////////////////////////

	//x_k_i_j
	vector<vector<vector<IloNumVar>>> x;

	x.resize(graph->vehicles);
	for (k = 0; k<graph->vehicles ;k++){
		x[k].resize(graph->dimension);
		for (i = 0; i < graph->dimension; i++)
		{
			x[k][i].resize(graph->dimension);
		}
	}
	for (k = 0; k<graph->vehicles ;k++){
		for (i = 0; i < graph->dimension; i++) {
			for (j = 0; j < graph->dimension; j++) {
				x[k][i][j] = IloNumVar(env, 0.0, 1.0, ILOINT);
				ostringstream varname;
				varname.str("");
				varname << "x_" << k << "_" << i << "_" << j;
				x[k][i][j].setName(varname.str().c_str());
			}
		}
	}

	//////////////
	//////  CST
	//////////////

	IloRangeArray CC(env);

	// Constraint (1)
	for (k = 0; k<graph->vehicles ;k++){
		IloExpr c1(env);
		IloExpr c2(env);
		for (i = 1; i < graph->dimension; i++) {
			c1 += x[k][depot][i];
			c2 += x[k][i][depot];
		}
		CC.add(c1 == 1);
		CC.add(c2 == 1);
	}

	for (i = 1; i < graph->dimension; i++){
		IloExpr c3(env);
		IloExpr c4(env);
		for (k = 0; k<graph->vehicles ;k++) {
			c3 += x[k][depot][i];
			c4 += x[k][i][depot];
		}
		CC.add(c3 <= 1);
		CC.add(c4 <= 1);
	}
	

	// Constraints (3)
	for (i = 0; i < graph->dimension; i++) {
		if (i == depot) continue;
		IloExpr c5(env);
		IloExpr c6(env);
		for (k = 0; k < graph->vehicles ;k++) {
			for (j = 0; j < graph->dimension; j++) {
				if (j != i) {
					c5 += x[k][i][j];
					c6 += x[k][j][i];
				}
			}
		}
		CC.add(c5 == 1);
		CC.add(c6 == 1);
	}

	for (k = 0; k < graph->vehicles ;k++){
		for (i = 0; i < graph->dimension; i++) {
			if (i == depot) continue;
			IloExpr c7(env);
			IloExpr c8(env);
			for (j = 0; j < graph->dimension; j++) {
				if (j != i) {
					c7 += x[k][i][j];
					c8 += x[k][j][i];
				}
			}
			CC.add(c7 <= 1);
			CC.add(c8 <= 1);
		}
	}


	// Trivial Constraints
	// Diagonal = 0
	for (k = 0; k<graph->vehicles ;k++) {
		for (i = 0; i < graph->dimension; i++) {
			CC.add(x[k][i][i] == 0);
		}
	}

	model.add(CC);


	//////////////
	////// OBJ
	//////////////

	IloObjective obj = IloAdd(model, IloMinimize(env, 0.0));

	for (k = 0; k<graph->vehicles ;k++)
		for (i = 0; i < graph->dimension; i++)
			for (j = 0; j < graph->dimension; j++)
				obj.setLinearCoef(x[k][i][j], graph->distance(i+1,j+1) + graph->getReward(k,j+1));

	///////////
	//// RESOLUTION
	//////////

	IloCplex cplex(model);

	/// ADD SEPARATION CALLBACK
	cplex.use(LazyWeightCutSeparationCst(env,*graph,x));

	// cplex.setParam(IloCplex::Cliques,-1);
	// cplex.setParam(IloCplex::Covers,-1);
	// cplex.setParam(IloCplex::DisjCuts,-1);
	// cplex.setParam(IloCplex::FlowCovers,-1);
	// cplex.setParam(IloCplex::FlowPaths,-1);
	// cplex.setParam(IloCplex::FracCuts,-1);
	// cplex.setParam(IloCplex::GUBCovers,-1);
	// cplex.setParam(IloCplex::ImplBd,-1);
	// cplex.setParam(IloCplex::MIRCuts,-1);
	// cplex.setParam(IloCplex::ZeroHalfCuts,-1);
	// cplex.setParam(IloCplex::MCFCuts,-1);
	// cplex.setParam(IloCplex::MIPInterval,1);
	// cplex.setParam(IloCplex::HeurFreq,-1);
	// cplex.setParam(IloCplex::ClockType,1);
	// cplex.setParam(IloCplex::RINSHeur,-1);
	// cplex.setParam(IloCplex::Param::MIP::Cuts::LiftProj,-1);


	cplex.exportModel("sortie.lp");

	try{
		if (!cplex.solve()) {
			env.error() << "Failed to optimize LP" << endl;
			exit(1);
		}
	}
	catch (IloException& e) {
	  	cerr << e.getMessage();
	  	e.end();
	}


	env.out() << "Solution status = " << cplex.getStatus() << endl;
	env.out() << "Solution value  = " << cplex.getObjValue() << endl;


	// vector<vector<int>>   solx;
	// solx.resize(graph->dimension);
	// for (i=0;i<graph->dimension;i++)
 //    	solx[i].resize(graph->dimension);

	// for (i = 0; i < graph->dimension; i++) {
	// 	for (j = 0; j < graph->dimension; j++) {
	// 		solx[i][j] = cplex.getValue(x[i][j]);
	// 	}
	// }

	// vector<vector<int>> bad_cycle;
 //  	vector<vector<int>> good_cycle;

	// getCycles(graph, &solx, &bad_cycle, &good_cycle);




	//////////////
	//////  CPLEX's ENDING
	//////////////

	env.end();

	//////////////
	//////  OUTPUT
	//////////////

	// for (int i = 0; i < good_cycle.size(); ++i)
	// {
	// 	for (int j = 0; j < good_cycle[i].size(); ++j)
	// 	{
	// 		good_cycle[i][j] += 1;
	// 	}
	// 	good_cycle[i].push_back(1);
	// }

	// graph->saveSolution(good_cycle,name + "sol.txt");


	/*ofstream ficsol(nameextsol.c_str());

	for (i = 0; i < G.nb_nodes; i++)
		ficsol << solx[i] << " ";

	ficsol.close();*/


	//return 0;
}

