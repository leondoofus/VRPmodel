#include <ilcplex/ilocplex.h>
#include <vector>
#include"../../Graph/Graph.h"

#define epsilon 0.00001

using namespace::std;


//Find a violated "Menger" Cut by a separation algorithm
bool  find_ViolatedMengerCutCst(IloEnv env, C_Graph & G,  vector<vector<IloNumVar> >& x,  IloRange & ViolatedCst){

  int i,j;
  list<int> W;
  list<int>::const_iterator it;
  vector<int> V_W;
  float test;

  V_W.resize(G.nb_nodes);

  // Find a minimum cut


  test=G.Undirected_MinimumCut(W);

  //cout<<"test = "<<test<<endl;
  
  if (test<2-epsilon) {
    // Found a violated inequality 
      
    IloExpr expr(env);
    for (i=0;i<G.nb_nodes;i++)  V_W[i]=0;
    for (it=W.begin();it!=W.end();it++) V_W[*it]=1;

    // cout<<"Set W found :";
    // for (it=W.begin();it!=W.end();it++)
    //   cout<<*it<<" ";
    // cout<<endl;
    
    for (it=W.begin();it!=W.end();it++)
      for (j=0;j<G.nb_nodes;j++)
    	if (V_W[j]==0) expr+=x[*it][j];

      
    ViolatedCst=IloRange(expr >= 2);
    return true;
  }
  else
    return false;

}



