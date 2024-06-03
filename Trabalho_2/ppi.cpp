#include <ilcplex/ilocplex.h>
#include <vector>
#include <fstream>


ILOSTLBEGIN
int main (int argc, char *argv[]) {
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
   
  IloCplex cplex(model); // declara variável "solver" sobre o modelo a ser solucionado
  cplex.solve();         // chama o "solver"
  cout << "Min=" << cplex.getObjValue() << endl; //  imprime solução do problema
  
  cplex.extract(model); // Extrai o modelo
  cplex.exportModel("recourse.lp"); // Retorna o modelo no arquivo recourse.lp
  env.end();
}
