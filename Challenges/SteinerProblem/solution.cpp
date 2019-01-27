#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include <vector>
#include <utility>
using namespace std;

const int MAX = 50;
int isTerminal[MAX];
int main() {

    IloEnv env;
    IloModel model(env, "Steiner Problem");
    IloCplex cplex(model);

    
    int v, e, t, v1, v2, w;
    pair<int, int> edges[MAX];
    int cost[MAX];

    scanf("%d %d", &v, &e);
    for(int i = 0; i < e; ++i){
        scanf("%d %d %d", &v1, &v2, &w);
        --v1,--v2; // enum from 0 to v-1
        edges[i].first = v1;
        edges[i].second = v2;
        cost[i] = w;
    }

    scanf("%d", &t);
    for(int i = 0; i < t; ++i){
        scanf("%d", &v1);
        --v1;
        isTerminal[v1] = 1;
    }
    
   // decision variables
   IloBoolVarArray eTree(env, e);
   IloBoolVarArray vTree(env, v);
   IloArray<IloNumVarArray> flow(env, e);

   for(int i = 0; i < e; ++i)flow[i] = IloNumVarArray(env, 2, 0, IloInfinity);
   
         // restrictions
        // every terminal must be on tree
        IloExpr totalTerminals(env);
        for(int i = 0; i < v; ++i)if(isTerminal[i]){
            totalTerminals+=vTree[i];
        }
        model.add(totalTerminals == t);
        // terminal must have at least one edge && non-termials must have at least two connections
        for(int i = 0; i < v; ++i){
            if(isTerminal[i]) {
                IloExpr total(env);
                for(int j = 0; j < e; ++j){
                    if(edges[j].first == i || edges[j].second == i){
                        total+= eTree[j];
                    }
                }
                model.add(total >= 1);
            } else {
                IloExpr total(env);
                for(int j = 0; j < e; ++j){
                    if(edges[j].first == i || edges[j].second == i){
                        total+= eTree[j];
                    }
                }
                model.add(total >= 2 * vTree[i]);
            }
        }
        // must be a tree
        IloExpr totalVertex(env);
        IloExpr totalEdges(env);
        for(int i = 0; i < e; ++i){
            totalEdges += eTree[i];
        }
        for(int i = 0; i < v; ++i){
            totalVertex+= vTree[i];
        }
        model.add(totalVertex == totalEdges + 1);
        // acyclic
        for(int i = 0; i < e; ++i){
            model.add(flow[i][0] + flow[i][1] == (2 * eTree[i])); 
        }
        
        IloExprArray flowsSum(env, v);
      
        for(int i = 0; i < v; ++i)flowsSum[i] = IloExpr(env);
        for(int i = 0; i < e; ++i){
            flowsSum[edges[i].first] += flow[i][0];
            flowsSum[edges[i].second] += flow[i][1];
        }
        for(int i = 0; i < v; ++i){
            model.add(flowsSum[i] <= (2.0 - 2.0/v));
        }


 
   //objective function

    IloExpr totalCost(env);
    for(int i = 0; i < e; ++i)totalCost += eTree[i] * cost[i];
    model.add(IloMinimize(env, totalCost));
   //results

    cplex.solve();

    printf("Minimum Cost = %.0lf\n", cplex.getObjValue());
    
    IloNumArray solutions(env, e);
    cplex.getValues(solutions, eTree);
    for(int i = 0; i < e; ++i){
        if(solutions[i])printf("%d - %d\n", edges[i].first+1, edges[i].second+1);
    }

    env.end();

    return 0;
}