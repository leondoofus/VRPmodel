#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>



#include"../../Graph/Graph.h"

#define epsilon 0.00001

using namespace std;

typedef IloArray<IloNumVarArray> NumVarMatrix;

///////////////////////////////
///////////////////////////////

// MIP formulation for the Coloring Problem
// Given a graph G=(V,E)

// sum_{k=0}^{K-1} w_k
// sum_{l=0}^{K-1} x^l_i=1 for every node i in V
// x^k_i + x^k_j <= w_l  for every edge ij in E and k in 0,...,K-1
// x^k_i in {0,1} for every node i in V and k in 0,...,K-1
// w_k for every k in 0,...,K-1

///////////////////////////////
///////////////////////////////

int main (int argc, char**argv){

  string name,nameext, nameextsol;
  int i,k,e;
  list<int>::const_iterator it;

  int K; // upper bound over the number of colors

  vector<int> sol;


  //////////////
  //////  DATA
  //////////////

  if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <DIMACS file name>   (without .dim)"<<endl; 
    return 1;
  }


  name=argv[1];
  nameext=name+".dim";
  nameextsol=name+".color";


  ifstream fic(nameext.c_str());

  if (fic==NULL){
    cerr<<"file "<<nameext<<" not found"<<endl;
    return 1;
  }

  C_Graph G;

  G.read_undirected_DIMACS(fic);

  fic.close();


  //////////////
  //////  CPLEX INITIALIZATION
  //////////////


  IloEnv   env;
  IloModel model(env);

  IloRangeArray c(env);


  ////////////////////////
  //////  VAR
  ////////////////////////

  K=G.nb_nodes;  // very bad upper bound !

  vector<IloNumVar> w;
  w.resize(K);
  for(k = 0; k < K; k++) {
    w[k]=IloNumVar(env, 0.0, 1.0, ILOINT);
    ostringstream varname;
    varname.str("");
    varname<<"w"<<k;
    w[k].setName(varname.str().c_str());
  }

  vector<vector<IloNumVar> >x;
  x.resize(K);
  
  for (k=0;k<K;k++){
    x[k].resize(G.nb_nodes);
    for(i = 0; i < G.nb_nodes; i++) {
      x[k][i]=IloNumVar(env, 0.0, 1.0, ILOINT);
      ostringstream varname;
      varname.str("");
      varname<<"x"<<k<<"_"<<i;
      x[k][i].setName(varname.str().c_str());
    }
  }


  //////////////
  //////  CST
  //////////////

  IloRangeArray CC(env);
  int nbcst=0;

  // sum_{k=0}^{K-1} x^k_i=1 for every node i in V

  for (i=0;i<G.nb_nodes;i++){
    IloExpr cst(env);
    for (k=0;k<K;k++)
      cst+=x[k][i];
    CC.add(cst==1);
    ostringstream cstname;
    cstname.str("");
    cstname<<"Cst_onecol_"<<i;
    CC[nbcst].setName(cstname.str().c_str());
    nbcst++;
  }

// x^k_i + x^k_j <= w_k  for every edge ij in E and k in 0,...,K-1

  for (k=0;k<K;k++)
    for (e=0;e<G.nb_links;e++){
      IloExpr cst(env);
      cst+= x[k][G.V_links[e]->v1] + x[k][G.V_links[e]->v2] - w[k];
      CC.add(cst<=0);
      ostringstream cstname;
      cstname.str("");
      cstname<<"Cst_edge_"<<k<<"_"<<G.V_links[e]->v1<<"_"<<G.V_links[e]->v2;
      CC[nbcst].setName(cstname.str().c_str());
      nbcst++;
  }

  model.add(CC);


  //////////////
  ////// OBJ
  //////////////
  
  IloObjective obj=IloAdd(model, IloMinimize(env, 0.0));

  for (k=0;k<K;k++)
    obj.setLinearCoef(w[k],1.0);
  

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


  int couleur=0;
  vector<int>   solx;
  solx.resize(G.nb_nodes);
  for (k=0;k<K;k++) {
    if (cplex.getValue(w[k])>1-epsilon){
      couleur++;
      for(i = 0; i < G.nb_nodes; i++) {
	if (cplex.getValue(x[k][i])>1-epsilon)
	  solx[i]=couleur;
      }
    }
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
