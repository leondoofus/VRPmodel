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
	int index = distance(solx.begin(),find(solx.begin(), solx.end(), 1));
	if (find(cycle.begin(), cycle.end(), index) != cycle.end())
	{
		index = distance(solx.begin(),find(solx.begin()+index+1, solx.end(), 1));
		if (find(cycle.begin(), cycle.end(), index) != cycle.end())
		{
			return -1;
		}
		return index;
	}
	return index;
}

vector<int> getCycleFromIndex (vector<vector<int>> solx, int index)
{
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

bool getCycles (Graph* G, vector<vector<int>>* solx, vector<vector<int>>* bad_cycle, vector<vector<int>>* good_cycle)
{
	bool detect = false;
	int i,j;

	vector<int> checked;

	for (i=1;i<G->dimension;i++)
	{
		if(find(checked.begin(), checked.end(), i) == checked.end()) // i not checked
		{
			vector<int> c = getCycleFromIndex(*solx, i);
			if(find(c.begin(), c.end(), 0) == c.end()) //cycle doesn't pass by depot
			{
				bad_cycle->push_back(c);
				detect = true;
			}
			else
				good_cycle->push_back(c);
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
		int W = 2 * (int)ceil(weight/(double)Q);
		IloRange ViolatedCst = IloRange(cviolated >= W);
		//cout << ViolatedCst << endl;
		add(ViolatedCst,IloCplex::UseCutForce);
  	}
  }else{
  	#ifdef OUTPUT
  	cout << "NO CYCLE OUTSIDE DETECTED" << endl;
  	#endif
  }

  #ifdef OUTPUT
  cout << "CHECKING " << good_cycle.size() << " GOOD CYCLE(S) ... " << endl;
  #endif

  // for (i=0;i<good_cycle.size();i++)
  // 	{
  // 		double weight = 0.0;
  // 		vector<int> c = good_cycle.at(i);
  // 		if (c.size() == G.dimension) break;
  // 		IloExpr cviolated(getEnv());
  // 		for (j = 0; j < G.dimension; j++) {
  // 			if (find(c.begin(), c.end(), j) == c.end())
	 //  			weight += (double)G.demand[j+1];
		// 		for (int k = 0; k < c.size(); k++) {
		// 				cviolated += x[j][c.at(k)];				}
		// }
		// int W = 2 * (int)ceil(weight/(double)Q);
		// IloRange ViolatedCst = IloRange(cviolated >= W);
		// //cout << ViolatedCst << endl;
		// add(ViolatedCst,IloCplex::UseCutForce);
  // 	}

	// for (i=0;i<good_cycle.size();i++)
 //  	{
 //  		int demand = 0;
 //  		vector<int> c = good_cycle.at(i);
 //  		for (j = 0; j < c.size(); j++) {
 //  			demand += G.demand[c.at(j)+1];
	// 	}
	// 	if (demand > Q)
	// 	{
	// 		#ifdef OUTPUT
	// 		cout << "ADDING CONSTRAINT" << endl;
	// 		#endif

	// 		IloExpr cviolated(getEnv());
	// 		for (j = 0; j < c.size(); j++)
	// 			for (int k = 0; k < c.size(); k++)
	// 				cviolated += x[c.at(j)][c.at(k)];
	// 		int size = 2 * c.size() - 2;
	// 		IloRange ViolatedCst = IloRange(cviolated <= size);
	// 		//cout << ViolatedCst << endl;
	// 		add(ViolatedCst,IloCplex::UseCutForce);

	// 	}
 //  	}

  	#ifdef OUTPUT
  	cout<<"*********** End Callback *************"<<endl;
  	#endif


  // int i;
  // list<C_link *>::const_iterator it;
  // IloRange ViolatedCst;
  
  // // Put the INTEGER values x on the edges of graph G
  // // Be carefull... Cplex can send -0.00001 for 0 or 0.000099 for 1

  // for (i=0;i<G.nb_nodes;i++)
  //   for (it=G.V_nodes[i].L_adjLinks.begin();it!=G.V_nodes[i].L_adjLinks.end();it++){
  //     if (i<(*it)->return_other_extrem(i)){
		// (*it)->algo_cost= getValue(x[i][(*it)->return_other_extrem(i)]);
		// if((*it)->algo_cost<epsilon) (*it)->algo_cost=0 ;
		// else (*it)->algo_cost=1 ;
  //     }
  //   }

  // /* Separation of Cut inequalities */

  // if (find_ViolatedMengerCutCst_INTEGER(getEnv(),G,x, ViolatedCst)){
  //   #ifdef OUTPUT
  //   cout << "Adding constraint : "<<endl;
  //   cout<< ViolatedCst << endl;
  //   #endif
  //   add(ViolatedCst,IloCplex::UseCutPurge);   // UseCutForce UseCutPurge UseCutFilter
  // }
  // #ifdef OUTPUT
  //   else {
  //     cout<<"No Cst found"<<endl;
  //   }
  // #endif 

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
			x[i][j] = IloNumVar(env, 0.0, 1.0, ILOINT);
			ostringstream varname;
			varname.str("");
			varname << "x_" << i << "_" << j;
			x[i][j].setName(varname.str().c_str());
		}
	}


	// vector<IloNumVar> w;
	// w.resize(graph->dimension);
	// for (i = 0; i < graph->dimension; i++) {
	// 	w[i] = IloNumVar(env, 0.0, Q, ILOFLOAT);
	// 	ostringstream varname;
	// 	varname.str("");
	// 	varname << "w_" << i << "_" << j;
	// 	w[i].setName(varname.str().c_str());
	// }

	//////////////
	//////  CST
	//////////////

	IloRangeArray CC(env);
	int nbcst = 0;
	//list<C_link*>::const_iterator it;

	// Constraint (1)
	
	IloExpr c1(env);
	for (i = 0; i < graph->dimension; i++) {
		if (i != depot) {
			c1 += x[depot][i];
		}
	}
	CC.add(c1 <= graph->vehicles);
	CC[nbcst].setName("VehiclesOUT");
	nbcst++;

	// Constraints (3)
	for (i = 0; i < graph->dimension; i++) {
		IloExpr c3(env);
		for (j = 0; j < graph->dimension; j++) {
			if (j != i) {
				c3 += x[i][j];
			}
		}
		CC.add(c3 == 2);

		ostringstream nomcst;
		nomcst.str("");
		nomcst << "CstRetailerOUT_" << i;

		CC[nbcst].setName(nomcst.str().c_str());
		nbcst++;
	}


	// Constraints (5) MTZ
	// for (i = 0; i < graph->dimension; i++) {
	// 	for (j = 0; j < graph->dimension; j++) {
	// 		if (j != i) {
	// 			IloExpr c5(env);
	// 			c5 += w[i] - w[j] - (Q + graph->demand[i]) * x[i][j];
	// 			CC.add(c5 >= -Q);
	// 			ostringstream nomcst;
	// 			nomcst.str("");
	// 			nomcst << "CstMTZ_" << i << "_" << j;
	// 			CC[nbcst].setName(nomcst.str().c_str());
	// 			nbcst++;
	// 		}
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
	for (i = 0; i < graph->dimension; i++) {
		for (j = i+1; j < graph->dimension; j++) {
			IloExpr c7(env);
			c7 += x[i][j] - x[j][i];
			CC.add(c7 == 0);
			ostringstream nomcst;
			nomcst.str("");
			nomcst << "CstT2_" << i << "_" << j;
			CC[nbcst].setName(nomcst.str().c_str());
			nbcst++;
		}
	}


	model.add(CC);


	//////////////
	////// OBJ
	//////////////

	IloObjective obj = IloAdd(model, IloMinimize(env, 0.0));

	for (i = 0; i < graph->dimension; i++)
		for (j = i; j < graph->dimension; j++)
			obj.setLinearCoef(x[i][j], graph->distance(i+1,j+1));

	///////////
	//// RESOLUTION
	//////////

	IloCplex cplex(model);

	/// ADD SEPARATION CALLBACK
	//cplex.use(LazyWeightCutSeparation(env,*graph,x));

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


	if (!cplex.solve()) {
		env.error() << "Failed to optimize LP" << endl;
		exit(1);
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
			if (solx[i][j])
				cout << x[i][j].getName() << " " << solx[i][j] << endl;
		}
	}



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

