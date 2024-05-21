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

  int J, I, c, Q, n; 
  

  fstream file(argv[1], std::ios_base::in); // Leitura do arquivo


  file >> I >> J >> c >> Q >> n;


  std::vector<vector<float>> g(I, std::vector<float>(J, -1)); // Cria os vetores g_ij e p_ij de acordo com I e J
  std::vector<vector<int>> p(I, std::vector<int>(J, -1));

  /*
  Leitura do arquivo e preenchimento de p_ij e g_ij
  */
  int i,j;
  for (int k = 0; k < n; k++){
    
    file >> i >> j;
    i--;// Edita i e j para os indices iniciarem em 0
    j--;

    file >> g.at(i).at(j); 
    file >> p.at(i).at(j);
  }

  /*
  Modelo
  */

  /*
  Variaveis
  */
  IloBoolVarArray y(env, I); // variaveis y_i
  IloArray<IloBoolVarArray> x(env, I); // variaveis x_ij

  char varName[100];

  // Aloca y_i
  for(i=0; i < I;i++){ 
    sprintf(varName,"y_%d",i);
    y[i] = IloBoolVar(env, varName);
    model.add(y[i]);
  }

  // Aloca x_ij
  for(i=0; i < I; i++) { 
    x[i] = IloBoolVarArray(env, J);                
    for(j=0; j < J; j++) {
      if(g[i][j] > -1){ // Verifica se a aresta existe
        sprintf (varName, "x_%d_%d", i, j);   
        x[i][j] = IloBoolVar(env, varName);            
        model.add(x[i][j]);    
      }
                       
    }
  }

  /*
  Restricoes
  */

  // Somatorio x_ij = 1, para todo j em J
  IloConstraintArray constraint1_array(env);
  for(j = 0; j < J; j++){
    IloExpr constraint(env); 
    for (i=0; i < I; i++){
      if(g[i][j] > -1){
        constraint += x[i][j];
      }
    }
    constraint1_array.add(constraint == 1);
    constraint.end();
  }
  model.add(constraint1_array);

  // x_ij <= y_i
  for(i=0; i < I; i++) {             
    for(j=0; j < J; j++) {
      if(g[i][j] > -1){
        model.add(x[i][j]<=y[i]);  
      }   
    }
  }

  // Somatorio p_ij * x_ij <= Q * y_i, para todo i em I
  IloConstraintArray constraint2_array(env);
  for(i=0; i < I; i++) {  
    IloExpr constraint(env); 
    for(j=0; j < J; j++) {
      if(g[i][j] > -1){
        constraint += p[i][j]*x[i][j];
      }
    }
    constraint2_array.add(constraint <= Q*y[i]);
    constraint.end();
  }
  model.add(constraint2_array);

  /*
  Funcao objetivo
  */
  IloExpr funcObj(env);

  for (i=0; i < I; i++){
    funcObj += c*y[i];
  }

  for(i=0; i < I; i++) {             
    for(j=0; j < J; j++) {
      if(g[i][j] > -1){
        funcObj += g[i][j]*x[i][j];  
      }   
    }
  }

  model.add(IloMinimize(env,funcObj,"f"));
   
  IloCplex cplex(model); // declara variável "solver" sobre o modelo a ser solucionado
  cplex.solve();         // chama o "solver"
  cout << "Min=" << cplex.getObjValue() << endl; //  imprime solução do problema
  
  cplex.extract(model); // Extrai o modelo
  cplex.exportModel("recourse.lp"); // Retorna o modelo no arquivo recourse.lp
  env.end();
}
