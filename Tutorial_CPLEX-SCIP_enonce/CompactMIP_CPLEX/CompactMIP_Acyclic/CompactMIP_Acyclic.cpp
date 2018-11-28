#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include"../../Graph/Graph.h"

#define epsilon 0.00001

using namespace std;

///////////////////////////////
///////////////////////////////

// Compact MIP formulation for the Acyclic Induced Subgraph Problem
// Given a directed graph G=(V,A)

// sum_{i in V} x_i
// u_i -u_j +1 <= n (2 -x_i -x_j) for all arc (i,j) in A
// 1<=u_i<=n for all i\in V
// u_i real for all i\in V
// x_i in {0,1} for every node i in V

///////////////////////////////
///////////////////////////////

int main (int argc, char**argv){

  string name, nameext, nameextsol;
  int i,k;

  vector<int> sol;


  //////////////
  //////  DATA
  //////////////

  if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <GRA file name>   (without .gra)"<<endl; 
    return 1;
  }

  name=argv[1];
  nameext=name+".gra";
  nameextsol=name+".acycl";


  ifstream fic(nameext.c_str());

  if (fic==NULL){
    cerr<<"file "<<nameext<<" not found"<<endl;
    return 1;
  }

  C_Graph G;

  G.read_directed_GRA(fic);

  fic.close();


  //////////////
  //////  CPLEX INITIALIZATION
  //////////////


  IloEnv   env;
  IloModel model(env);



  ////////////////////////
  //////  VAR
  ////////////////////////

  vector<IloNumVar> x;
  x.resize(G.nb_nodes);
  for(i = 0; i < G.nb_nodes; i++) {
    x[i]=IloNumVar(env, 0.0, 1.0, ILOINT);
    ostringstream varname;
    varname.str("");
    varname<<"x_"<<i;
    x[i].setName(varname.str().c_str());
  }

  vector<IloNumVar> u;
  u.resize(G.nb_nodes);
  for(i = 0; i < G.nb_nodes; i++) {
    u[i]=IloNumVar(env, 1.0, G.nb_nodes, ILOFLOAT);
    ostringstream nomvar;
    nomvar.str("");
    nomvar<<"u_"<<i;
    u[i].setName(nomvar.str().c_str());
  }



  //////////////
  //////  CST
  //////////////

  IloRangeArray CC(env);
  int nbcst=0;
  list<C_link*>::const_iterator it;

  // Cte u_i -u_j +1 <= n (2 -xi -xj)
  // becomes  n xi + n xj +ui -uj <= 2n -1

  for (i=0;i<G.nb_nodes;i++){
    for (it=G.V_nodes[i].L_adjLinks.begin();it!=G.V_nodes[i].L_adjLinks.end();it++){
      IloExpr c1(env);
      c1+=G.nb_nodes*x[i]+G.nb_nodes*x[(*it)->v2]+u[i]-u[(*it)->v2];
      CC.add(c1<=2*G.nb_nodes-1);
      ostringstream nomcst;
      nomcst.str("");
      nomcst<<"CstMTZ_"<<i<<"_"<<(*it)->v2;
      CC[nbcst].setName(nomcst.str().c_str());
      nbcst++;
    }
  }


  model.add(CC);


  //////////////
  ////// OBJ
  //////////////
  
  IloObjective obj=IloAdd(model, IloMaximize(env, 0.0));
  
  for (i=0;i<G.nb_nodes;i++)
    obj.setLinearCoef(x[i],G.V_nodes[i].weight);
 

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


  if ( !cplex.solve() ) {
    env.error() << "Failed to optimize LP" << endl;
    exit(1);
  }

 
  env.out() << "Solution status = " << cplex.getStatus() << endl;
  env.out() << "Solution value  = " << cplex.getObjValue() << endl;


  vector<int>   solx;
  solx.resize(G.nb_nodes);
  for(i = 0; i < G.nb_nodes; i++) {
    solx[i]=cplex.getValue(x[i]);
  }  


  //////////////
  //////  CPLEX's ENDING
  //////////////

  env.end();

  //////////////
  //////  OUTPUT
  //////////////


  ofstream ficsol(nameextsol.c_str());
  
  for(i = 0; i < G.nb_nodes; i++) 
    ficsol<<solx[i]<<" ";

  ficsol.close();


  return 0;
}
