#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <string>
#include <random>
#include "maxclique.h"

using namespace std;

double max_clique(int dim, double **graph, int *cutset) {
   /*
   Heuristica baseada em uma simplificacao do metodo exato de Branch & Bound apresentado em:
      Carraghan, Randy, and Panos M. Pardalos. "An exact algorithm for the maximum clique problem." Operations Research Letters 9.6 (1990): 375-382.
   */

   pair<int,int> vLen[dim]; // Par contendo o vertice e sua cardinalidade.

   vector<int> temp, tempCli, sortedV;

   vector<int> MaxCli;
   double MaxCliSize = 2;
   int i;

   float ratio = 0.5; // porcentagem de elementos testados


   // Ordena os elementos por ordem decrescente de cardinalidade
   for (int i= 0; i < dim; i++)
   {
      int sum = 0;
      for(int j = 0; j < dim; j++) // Encontrar vizinhos de i
      {
         if (graph[i][j] != 0) sum++;
      }
      vLen[i].second = i;
      vLen[i].first = sum;
   }

   // for (int i= 0; i < dim; i++){
   //    printf("v: %d, card: %d, ", vLen[i].second, vLen[i].first);
   // }
   // printf("\n");
   sort(vLen,vLen + dim);
   reverse(vLen,vLen + dim);

   for (int i= 0; i < dim; i++){
      sortedV.push_back(vLen[i].second); // gera um vetor com os vertices ordenados por cardinalidade.
   }



   for (int _= 0; _ < dim*ratio; _++)
   {
      i = rand() % dim; // escolhe elemento aleatorio
      for(int j = 0; j < dim; j++) // Encontrar vizinhos de i por ordem de cardinalidade
      { 
         if (graph[i][sortedV[j]] != 0) temp.push_back(sortedV[j]);
      }


      tempCli.push_back(i);
      while (temp.size() > 0)
      {
         tempCli.push_back(temp[0]); // elemento adicionado a clique

         if ((tempCli.size() + temp.size() - 1 ) <= MaxCliSize) // Caso não seja possível encontrar uma clique maior, poda.
         {
            temp.clear();
         }
         else
         {
         for (int k = 1; k < temp.size(); k++)
         {
            // printf("%f, ", graph[temp[0]][temp[k]]);
            if(graph[temp[0]][temp[k]] == 0){
                temp.erase(temp.begin() + k); // elemento removido caso tambem nao seja vizinho do elemento atual
                k--;
            }
         }
         // printf("\n");

         temp.erase(temp.begin());
         }
      }

      if(tempCli.size() > MaxCliSize){
         MaxCli = tempCli; 

         // for(int k = 0; k<tempCli.size(); k++){
         //    printf("%d, ", tempCli[k]);
         // }
         // printf("\n");

         MaxCliSize = tempCli.size();
      }

      tempCli.clear(); 
      temp.clear();
   }

   if(MaxCliSize < 3) return 0;

   for (int i = 0; i < MaxCli.size(); i++)
   {
      cutset[MaxCli[i]] = 1;
   }

   return MaxCliSize;
   }



