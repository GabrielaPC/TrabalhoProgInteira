#include <ilcplex/ilocplex.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <iostream> 



#include "maxclique.h"

#define PRECISAO 0.00001

using namespace std;

ILOUSERCUTCALLBACK5(CB_maxclique, IloArray<IloBoolVarArray>, x, int, dim, double**, residual, int*, cutset, int*, numc)
{
  /*
    Funcao baseada na implementacao presente no exemplo "6-cplex_exemplo_tsp_corte" disponibilizado no material disponibilizado pelo professor.
  */


  // x        -> vetor das variaveis do problema
  // dim      -> dimensao do problema (numero de cidades)
  // residual -> armazena grafo com a solucao corrente
  // cutset   -> conjunto S a ser encontrado
  // numc     -> contador de numero de cortes gerados

  IloEnv env  = getEnv();
  bool   DEPU = false;


  // cria grafo da solucao corrente fracionaria 
  for(int i=0; i < dim; i++) {
    for(int j=i; j < dim; j++) {
      residual[i][j] = 0;
      residual[j][i] = 0;
    }
  }

  for(int i=0; i < dim-1; i++) {
    for(int j=i+1; j < dim; j++) {
      float x_ij = getValue(x[i][j]);

      if (x_ij < PRECISAO) 
        x_ij = 0;

      residual[i][j] = x_ij;
      residual[j][i] = x_ij;
    }
  }

  // onde o corte sera armazenado (conjunto S)
  for(int i=0; i < dim; i++)
    cutset[i] = 0;

  // heurisitica para clique maxima
  double maxClique;
  maxClique = max_clique(dim, residual, cutset);

  // gera corte
  if (DEPU) cout<<"MaxClique="<<maxClique<<endl;
  if(maxClique > 2) {

    IloExpr corte(env);
    double  val=0;
    for(int i=0; i < dim; i++) {
      for(int j=i+1; j < dim; j++) {
    if(cutset[i] == 1 && cutset[j] == 1) {
      corte += x[i][j];
    }
      }
    }
    add(corte <= maxClique-1).end();
    *numc = *numc + 1;

    // libera memoria
    corte.end();

    double ub     = getIncumbentObjValue();  // retorna a melhor solucao inteira (limite primal)
    double lb     = getBestObjValue();       // retorna o melhor limite dual
    double rlx    = getObjValue();           // quando chamada dentro do callback, retorna o valor da relaxacao do noh
    double nNodes = getNremainingNodes();    // retorna o numero restante de nos a serem analisados
    cout<<"--- USER CUT:"<<"relax="<<rlx<<"\t bounds="<<lb<<"<->"<<ub<<"\t n_rest="<<nNodes<<"\t Ucuts="<<*numc<<endl;

  }

}

/*
Codigo do segundo trabalho
*/
ILOSTLBEGIN
int main (int argc, char *argv[]) {
  clock_t start;
  start = clock();

  IloEnv             env;   // declara Variável de ambiente do CPLEX
  IloModel     model(env);  // declara Variavel do modelo do CPLEX (que existe dentro do ambiente criado)
  
  /*
  Variaveis fixas do problema
  */

  int n,m; 

  string trash; // variavel para armazenar as strings a serem ignoradas

  

  fstream file(argv[1], std::ios_base::in); // Leitura do arquivo


  file >> trash >> trash >> n >> m;

  // printf("%s %d %d \n", trash.c_str(), n, m);

  std::vector<vector<int>> e(n, std::vector<int>(n, 0)); // Cria um vetor e_ij para armazenar as arestas.


  /*
  Leitura do arquivo e preenchimento de e_ij
  */
  int i,j,k;
  for (k = 0; k < m; k++){
    
    file >> trash >> i >> j;

    // printf("%s %d %d \n", trash.c_str(), i, j);

    i--;// Edita i e j para os indices iniciarem em 0
    j--;

    e[i][j] = 1;
  }


  // /*
  // Modelo
  // */

  /*
  Variaveis
  */
  IloBoolVarArray w(env, n); // variaveis w_i
  IloArray<IloBoolVarArray> x(env, n); // variaveis x_ij

  char varName[100];

  // Aloca w_i
  for(i=0; i < n;i++){ 
    sprintf(varName,"w_%d",i);
    w[i] = IloBoolVar(env, varName);
    model.add(w[i]);
  }

  // Aloca x_ij
  for(i=0; i < n; i++) { 
    x[i] = IloBoolVarArray(env, n);                
    for(j=0; j < n; j++) {
        sprintf (varName, "x_%d_%d", i, j);   
        x[i][j] = IloBoolVar(env, varName);            
        model.add(x[i][j]);    
      }
                       
    }

  // /*
  // Restricoes
  // */

  // Somatorio x_ij = 1, para todo i em V
  IloConstraintArray constraint1_array(env);
  for(i = 0; i < n; i++){
    IloExpr constraint(env); 
    for (j=0; j < n; j++){
        constraint += x[i][j];
    }
    constraint1_array.add(constraint == 1);
    constraint.end();
  }
  model.add(constraint1_array);

  // x_ij + x_kj <= w_i
  for(i=0; i < n; i++) {             
    for(k=0; k < n; k++) {
      if(e[i][k] == 1){
        for(j=0; j < n; j++) {
          model.add(x[i][j] + x[k][j] <=w[j]);  
        }
      }   
    }
  }

  // w_j >= w_(j+1)
  for(j=0; j < n -1; j++) {             
    model.add(w[j] >=w[j+1]);
  }

  // w_j <= somatorio ij
  IloConstraintArray constraint2_array(env);
  for(j=0; j < n -1; j++) { 
    IloExpr constraint(env);   
    for(i=0; i < n; i++) {         
      constraint += x[i][j];
    }
    constraint2_array.add(w[j] <=constraint);
    constraint.end();
  }
  model.add(constraint2_array);

  // /*
  // Funcao objetivo
  // */
  IloExpr funcObj(env);

  for (j=0; j < n; j++){
    funcObj += w[j];
  }

  model.add(IloMinimize(env,funcObj,"f"));


  // ***********************  callback de corte ******************************

  // estrutara usada para os métodos de geracao de cortes 
  double **residual = new double*[n];               // grafo residual usado no corte
  for(int i=0; i < n; i++)
    residual[i] = new double[n];
  int *cutset = new int[n];
  int num_u=0;



  IloCplex cplex(model); // declara variável "solver" sobre o modelo a ser solucionado
  cplex.extract(model); // Extrai o modelo
  cplex.exportModel("recourse.lp"); // Retorna o modelo no arquivo recourse.lp


  // -- USER CUT MAX CLIQUE --
  if(strcmp(argv[2], "0") != 0){
    cplex.use(CB_maxclique(env, x, n, residual, cutset, &num_u));  
  }

  // Parametros do CPLEX
  cplex.setParam(IloCplex::Param::MIP::Strategy::File, 3);   //quando a RAM acaba, 1-guarda nos na memoria e compactado 2-guarda os nos em disco 3-guarda nos em disco e compactado
  cplex.setParam(IloCplex::Param::TimeLimit, 3600*24);          // tempo limite
  cplex.setParam(IloCplex::Param::Threads, 6);               // Numero de threads
  cplex.setParam(IloCplex::Param::MIP::Interval, 100);       // Log a cada N nos
  // cplex.setOut(env.getNullStream());        // Desabilitando saida do cplex

   
  // Parametros do B&C
  cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0);        // desliga o preprocessamento, 1-ligado 0-desligado
  cplex.setParam(IloCplex::Param::MIP::Limits::CutsFactor, 0);        // número de cortes que o CPLEX pode gerar, 0-desliga os cortes
  cplex.setParam(IloCplex::Param::MIP::Strategy::HeuristicFreq, -1);  // heuristicas primais do CPLEX, -1-desliga 0-liga

  // Otimiza
    try
    {
	    cplex.solve();
    }
    catch(IloCplex::Exception &e)
    {
	    cout << e;
    }


    // Saida
    bool   solved;
    int    nNodes;
    double res, lb, ub, time_s;
    try
    {

    if(cplex.getStatus() == IloAlgorithm::Optimal)
      solved = true;
    else
      solved = false;

    cout<<"----- ACABOU ----"<<endl;
    if (solved)
    {
        cout<<"Resolveu?: Sim"<<endl;
    }else{
        cout<<"Resolveu?: Nao"<<endl;
    }

    time_s = ((double)clock() - (double)start) / CLOCKS_PER_SEC;  // tempo de processamento
    cout<<"Tempo(s): "<<time_s<<endl;

    lb     = cplex.getBestObjValue();                            // limite dual (inferior)
    cout<<"Limite inferior: "<<lb<<endl;

    res    = cplex.getObjValue();                               // solução
    cout<<"Minimo encontrado: "<<res<<endl;

    cout<<"Cortes gerados: "<<num_u<<endl;

    nNodes = cplex.getNnodes();                                  // numero de nos analisados na arvore de busca
    cout<<"Nos analisados: "<<nNodes<<endl;
    cout<<endl;

    
    }
      catch(IloCplex::Exception &e)
    {
      cout << e;
    }

  // libera memoria
  delete [] cutset;
  for(int i=0; i < n; i++)
    delete [] residual[i];
  delete [] residual;
  
  env.end();
}
