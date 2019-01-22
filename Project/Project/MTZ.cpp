#include "MTZ.h"
#include <ilcplex/ilocplex.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>


#define epsilon 0.00001

using namespace std;

#define OUTPUT


int getNeighbour (vector<int> solx, vector<int> cycle)
{
	// cout << "backkk " << cycle.back() << endl;
	int index = distance(solx.begin(),find(solx.begin(), solx.end(), 1));
	if (find(cycle.begin(), cycle.end(), index) != cycle.end())
	{
		return -1;
	}
	return index;
}

vector<int> getCycleFromIndex (vector<vector<int>> solx, int index)
{
	// cout << "Getting Cycle From " << index << " : ";
	vector<int> cycle;
	cycle.push_back(index);
	int neighbour = getNeighbour(solx.at(cycle.back()), cycle);
	while (neighbour != -1)
	{
		cycle.push_back(neighbour);
		neighbour = getNeighbour(solx.at(cycle.back()), cycle);
	}
	#ifdef OUTPUT
	cout << "Getting Cycle From " << index << " : ";
	for (int i=0; i<cycle.size(); i++) cout << cycle.at(i) << " ";
	cout << endl;
	#endif
	return cycle;
}

vector<int> getGoodCycleFromIndex (vector<vector<int>> solx, int index)
{
	cout << "Getting Cycle From " << index << " : ";
	vector<int> cycle;
	cycle.push_back(0);
	cycle.push_back(index);
	int neighbour = getNeighbour(solx.at(cycle.back()), cycle);
	while (neighbour != -1)
	{
		cycle.push_back(neighbour);
		neighbour = getNeighbour(solx.at(cycle.back()), cycle);
	}
	#ifdef OUTPUT
	//cout << "Getting Cycle From " << index << " : ";
	for (int i=0; i<cycle.size(); i++) cout << cycle.at(i) << " ";
	cout << endl;
	#endif
	return cycle;
}

bool getCycles (Graph* G, vector<vector<int>>* solx, vector<vector<int>>* bad_cycle, vector<vector<int>>* good_cycle)
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

ILOLAZYCONSTRAINTCALLBACK2(LazyWeightCutSeparation,
			   Graph &, G,			 
			   vector<vector<IloNumVar>>&,x
		    ){
  #ifdef OUTPUT
  cout<<"*********** Lazy separation Callback *************"<<endl;
  #endif

	int i,j;
	int Q = G.capacity;
	vector<vector<int>> solx;
	solx.resize(G.dimension);
	for (i=0;i<G.dimension;i++)
		solx[i].resize(G.dimension);
	for (i = 0; i < G.dimension; i++)
		for (j = 0; j < G.dimension; j++)
			solx[i][j] = getValue(x[i][j]);

	for (i = 0; i < G.dimension; i++){
		if (i<10) cout << i << " ";
		else cout << i;}
	cout <<endl;
	for (i = 0; i < G.dimension; i++){
		for (j = 0; j < G.dimension; j++){
			cout << solx[i][j] << " ";
		}
		cout << endl;
	}

  vector<vector<int>> bad_cycle;
  vector<vector<int>> good_cycle;

  if (getCycles(&G, &solx, &bad_cycle, &good_cycle))
  {
  	#ifdef OUTPUT
  	cout << "CYCLE(S) OUTSIDE DETECTED, ADDING " << bad_cycle.size() << " CONSTRAINTS" << endl;
  	#endif
  	for (i=0;i<bad_cycle.size();i++)
  	{
  		double weight = 0.0;
  		vector<int> c = bad_cycle.at(i);
  		IloExpr cviolated(getEnv());
  		for (j = 0; j < c.size(); j++) {
  			weight += (double)G.demand[c.at(j)+1];
			for (int k = 0; k < G.dimension; k++) {
				if (find(c.begin(), c.end(), k) == c.end())
					cviolated += x[c.at(j)][k];
			}
		}
		int W = (int)ceil(weight/(double)Q);
		IloRange ViolatedCst = IloRange(cviolated >= W);
		//cout << ViolatedCst << endl;
		add(ViolatedCst,IloCplex::UseCutPurge);
  	}
  }else{
  	#ifdef OUTPUT
  	cout << "NO CYCLE OUTSIDE DETECTED" << endl;
  	#endif
  }
	cout << "bad size " << bad_cycle.size() << endl;
	cout << "good size " << good_cycle.size() << endl;

  #ifdef OUTPUT
  cout << "CHECKING " << good_cycle.size() << " GOOD CYCLE(S) ... " << endl;
  #endif

  for (i=0;i<good_cycle.size();i++)
  	{
  		double weight = 0.0;
  		vector<int> c = good_cycle.at(i);
  		IloExpr cviolated(getEnv());
  		for (j = 1; j < c.size(); j++) {
  			weight += (double)G.demand[c.at(j)+1];
			for (int k = 0; k < G.dimension; k++) {
				if (k == 0) cviolated += x[c.at(j)][k];
				else {
					if (find(c.begin(), c.end(), k) == c.end())
						cviolated += x[c.at(j)][k];
				}
			}
		}
		if (weight > (double)Q) {
			int W = (int)ceil(weight/(double)Q);
			IloRange ViolatedCst = IloRange(cviolated >= W);
			//cout << ViolatedCst << endl;
			add(ViolatedCst,IloCplex::UseCutPurge);
			#ifdef OUTPUT
		  	cout << "Weight = " << weight <<" ADDING 1 CONSTRAINT" << endl;
		  	#endif
		}
  	}

}








void MTZ::compute(Graph *graph) { //undirected graph
	string name, nameext, nameextsol;
	int i, j, k;

	int depot = graph->depot - 1; //Reformule with an offset of 1
	int Q = graph->capacity;

	vector<int> sol;


	//////////////
	//////  DATA
	//////////////

	/*if (argc != 2) {
		cerr << "usage: " << argv[0] << " <GRA file name>   (without .gra)" << endl;
		return 1;
	}*/

	name = graph->name;
	nameext = name + ".gra";
	nameextsol = name + ".acycl";


	/*ifstream fic(nameext.c_str());

	if (fic == NULL) {
		cerr << "file " << nameext << " not found" << endl;
		return 1;
	}

	C_Graph G;

	G.read_directed_GRA(fic);

	fic.close();*/


	//////////////
	//////  CPLEX INITIALIZATION
	//////////////


	IloEnv   env;
	IloModel model(env);


	////////////////////////
	//////  VAR
	////////////////////////

	vector<vector<IloNumVar>> x;
	x.resize(graph->dimension);
	for (i=0;i<graph->dimension;i++)
    	x[i].resize(graph->dimension);

	for (i = 0; i < graph->dimension; i++) {
		for (j = 0; j < graph->dimension; j++) {
			x[i][j] = IloNumVar(env, 0.0, 1.0, ILOBOOL);
			ostringstream varname;
			varname.str("");
			varname << "x_" << i << "_" << j;
			x[i][j].setName(varname.str().c_str());
		}
	}


	// vector<IloNumVar> w;
	// w.resize(graph->dimension);
	// for (i = 0; i < graph->dimension; i++) {
	//  	w[i] = IloNumVar(env, 0.0, Q, ILOFLOAT);
	// 	ostringstream varname;
	//  	varname.str("");
	// 	varname << "w_" << i << "_" << j;
	//  	w[i].setName(varname.str().c_str());
	// }

	//////////////
	//////  CST
	//////////////

	IloRangeArray CC(env);
	int nbcst = 0;
	//list<C_link*>::const_iterator it;

	// Constraint (1)
	
	IloExpr c1(env);
	IloExpr c2(env);
	for (i = 0; i < graph->dimension; i++) {
		if (i != depot) {
			c1 += x[depot][i];
			c2 += x[i][depot];
		}
	}
	CC.add(c1 <= graph->vehicles);
	CC[nbcst].setName("VehiclesOUT");
	nbcst++;
	CC.add(c2 <= graph->vehicles);
	CC[nbcst].setName("VehiclesIN");
	nbcst++;

	// Constraints (3)
	for (i = 0; i < graph->dimension; i++) {
		if (i == depot) continue;
		IloExpr c3(env);
		IloExpr c4(env);
		for (j = 0; j < graph->dimension; j++) {
			if (j != i) {
				c3 += x[i][j];
				c4 += x[j][i];
			}
		}
		CC.add(c3 == 1);

		ostringstream nomcst;
		nomcst.str("");
		nomcst << "CstRetailerOUT_" << i;

		CC[nbcst].setName(nomcst.str().c_str());
		nbcst++;

		CC.add(c4 == 1);

		nomcst.str("");
		nomcst << "CstRetailerIN_" << i;

		CC[nbcst].setName(nomcst.str().c_str());
		nbcst++;
	}


	// Constraints (5) MTZ
	// for (i = 0; i < graph->dimension; i++) {
	//  	for (j = 0; j < graph->dimension; j++) {
	//  		if (j != i) {
	//  			IloExpr c5(env);
	//  			c5 += w[i] - w[j] - (Q + graph->demand[i+1]) * x[i][j];
	//  			CC.add(c5 >= -Q);
	//  			ostringstream nomcst;
	//  			nomcst.str("");
	// 			nomcst << "CstMTZ_" << i << "_" << j;
	//  			CC[nbcst].setName(nomcst.str().c_str());
	// 			nbcst++;
	//  		}
	// 	}
	// }

	// Trivial Constraints
	// Diagonal = 0
	for (i = 0; i < graph->dimension; i++) {
		CC.add(x[i][i] == 0);
		ostringstream nomcst;
		nomcst.str("");
		nomcst << "CstT_" << i;
		CC[nbcst].setName(nomcst.str().c_str());
		nbcst++;
	}

	// Symmetry
	// for (i = 0; i < graph->dimension; i++) {
	// 	for (j = i+1; j < graph->dimension; j++) {
	// 		IloExpr c7(env);
	// 		c7 += x[i][j] - x[j][i];
	// 		CC.add(c7 == 0);
	// 		ostringstream nomcst;
	// 		nomcst.str("");
	// 		nomcst << "CstT2_" << i << "_" << j;
	// 		CC[nbcst].setName(nomcst.str().c_str());
	// 		nbcst++;
	// 	}
	// }


	model.add(CC);


	//////////////
	////// OBJ
	//////////////

	IloObjective obj = IloAdd(model, IloMinimize(env, 0.0));

	for (i = 0; i < graph->dimension; i++)
		for (j = 0; j < graph->dimension; j++)
			obj.setLinearCoef(x[i][j], graph->distance(i+1,j+1));

	///////////
	//// RESOLUTION
	//////////

	IloCplex cplex(model);

	/// ADD SEPARATION CALLBACK
	 cplex.use(LazyWeightCutSeparation(env,*graph,x));

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


	vector<vector<int>>   solx;
	solx.resize(graph->dimension);
	for (i=0;i<graph->dimension;i++)
    	solx[i].resize(graph->dimension);

	for (i = 0; i < graph->dimension; i++) {
		for (j = 0; j < graph->dimension; j++) {
			solx[i][j] = cplex.getValue(x[i][j]);
			// if (solx[i][j])
			// 	cout << x[i][j].getName() << " " << solx[i][j] << endl;
		}
	}

	vector<vector<int>> bad_cycle;
  	vector<vector<int>> good_cycle;

	getCycles(graph, &solx, &bad_cycle, &good_cycle);

	int weightTol = 0;
	double coutTol = 0;

	for (i=0;i<good_cycle.size();i++)
  	{
  		double weight = 0.0;
  		double cose = 0.0;
  		vector<int> c = good_cycle.at(i);
  		for (j = 0; j < c.size(); j++) {
  			weight += (double)graph->demand[c.at(j)+1];
  			if (j < c.size() - 1) cose += graph->distance(i+1,i+2);
  			else cose += graph->distance(i+1,1);
		}
		weightTol += weight;
		coutTol += cose;
		cout << "Weight = " << weight << endl;

  	}
  	cout << "weightTol " << weightTol << endl;
  	cout << "coutTol " << coutTol << endl;



	//////////////
	//////  CPLEX's ENDING
	//////////////

	env.end();

	//////////////
	//////  OUTPUT
	//////////////


	/*ofstream ficsol(nameextsol.c_str());

	for (i = 0; i < G.nb_nodes; i++)
		ficsol << solx[i] << " ";

	ficsol.close();*/


	//return 0;
}

