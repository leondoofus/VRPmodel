#include "MTZ.h"
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>


#define epsilon 0.00001

using namespace std;

void MTZ::compute(Graph *graph) {
	string name, nameext, nameextsol;
	int i, j, k;

	int depot = graph->depot; //Reformule with an offset of 1

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

	vector<IloNumVar> x;
	x.resize(graph->capacity*graph->capacity);
	for (i = 0; i < graph->capacity; i++) {
		for (j = 0; j < graph->capacity; j++) {
			x[i][j] = IloNumVar(env, 0.0, 1.0, ILOINT);
			ostringstream varname;
			varname.str("");
			varname << "x_" << i << "_" << j;
			x[i][j].setName(varname.str().c_str());
		}
	}

	/*vector<IloNumVar> u;
	u.resize(G.nb_nodes);
	for (i = 0; i < G.nb_nodes; i++) {
		u[i] = IloNumVar(env, 1.0, G.nb_nodes, ILOFLOAT);
		ostringstream nomvar;
		nomvar.str("");
		nomvar << "u_" << i;
		u[i].setName(nomvar.str().c_str());
	}*/



	//////////////
	//////  CST
	//////////////

	IloRangeArray CC(env);
	int nbcst = 0;
	//list<C_link*>::const_iterator it;

	// Constraints (1) and (2) of MTZ

	IloExpr c1(env);
	IloExpr c2(env);
	for (i = 0; i < graph->capacity; i++) {
		if (i != depot) {
			c1 += x[depot][i - 1];
			c2 += x[i - 1][depot];
		}
	}
	CC.add(c1 <= graph->vehicles);
	CC.add(c2 <= graph->vehicles);
	CC[nbcst].setName("VehiclesOUT");
	nbcst++;
	CC[nbcst].setName("VehiclesIN");
	nbcst++;

	// Constraints (3) and (4) of MTZ
	for (i = 0; i < graph->capacity; i++) {
		if (i != depot) {
			IloExpr c3(env);
			IloExpr c4(env);
			for (j = 0; j < graph->capacity; j++) {
				c3 += x[i][j];
				c4 += x[j][i];
			}
			CC.add(c3 <= 1);
			CC.add(c4 <= 1);

			ostringstream nomcst;
			nomcst.str("");
			nomcst << "CstRetailerOUT_" << i;

			CC[nbcst].setName(nomcst.str().c_str());
			nbcst++;

			ostringstream nomcst;
			nomcst.str("");
			nomcst << "CstRetailerIN_" << i;

			CC[nbcst].setName(nomcst.str().c_str());
			nbcst++;
		}
	}


	// Constraints (5)

	/*for (i = 0; i < graph->capacity; i++) {
		for (j = 0; j < graph->capacity; j++) {
			for (it = G.V_nodes[i].L_adjLinks.begin(); it != G.V_nodes[i].L_adjLinks.end(); it++) {
				IloExpr c1(env);
				c1 += G.nb_nodes*x[i] + G.nb_nodes*x[(*it)->v2] + u[i] - u[(*it)->v2];
				CC.add(c1 <= 2 * G.nb_nodes - 1);
				ostringstream nomcst;
				nomcst.str("");
				nomcst << "CstMTZ_" << i << "_" << (*it)->v2;
				CC[nbcst].setName(nomcst.str().c_str());
				nbcst++;
			}
		}
	}*/


	model.add(CC);


	//////////////
	////// OBJ
	//////////////

	IloObjective obj = IloAdd(model, IloMinimize(env, 0.0));

	for (i = 0; i < graph->capacity; i++)
		for (j = 0; j < graph->capacity; j++)
			obj.setLinearCoef(x[i][j], graph->distance(i+1,j+1));


	///////////
	//// RESOLUTION
	//////////

	IloCplex cplex(model);

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


	cplex.exportModel("sortie.lp");


	if (!cplex.solve()) {
		env.error() << "Failed to optimize LP" << endl;
		exit(1);
	}


	env.out() << "Solution status = " << cplex.getStatus() << endl;
	env.out() << "Solution value  = " << cplex.getObjValue() << endl;


	vector<int>   solx;
	solx.resize(graph->capacity*graph->capacity);
	for (i = 0; i < graph->capacity; i++) {
		for (j = 0; j < graph->capacity; j++)
			solx[i][j] = cplex.getValue(x[i][j]);
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

