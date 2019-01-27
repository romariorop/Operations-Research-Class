#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include <vector>
using namespace std;

const int MAX = 510;

int main() {

    IloEnv env;
    IloModel model(env, "Steiner Problem");
    IloCplex cplex(model);

    int isTerminal[MAX];
    int cost[MAX][MAX];
    int v, e, t, v1, v2, w;
    int start, top;

    scanf("%d %d", &v, &e);
    for(int i = 0; i < e; ++i){
        scanf("%d %d %d", &w, &v1, &v2);
        --v1,--v2; // enum from 0 to v-1
        cost[v1][v2] = cost[v2][v1] = w;
    }

    scanf("%d", &t);
    for(int i = 0; i < t; ++i){
        scanf("%d", &v1);
        isTerminal[v1] = 1;
    }

    // decision variables

    IloArray<IloIntVarArray> path(env, v);
    IloBoolVarArray onAns(env, v);
    for(int i = 0; i < v; ++i)path[i] = IloIntVarArray(env, v, 0, 1);

    //restrictions
        
        //graph restrictions
        for(int i = 0; i < v; ++i){
            model.add(path[i][i] == 0);
            for(int j = 0; j < v; ++j){
                model.add(path[i][j] == path[j][i]);   
            }
        }

        //all terminals must be in answer
        IloExpr totalTerminals(env);    
        for(int i = 0; i < v; ++i)totalTerminals += onAns[i] * isTerminal[i];
        model.add(totalTerminals == t);

        //all terminals must be linked to someone (unless there is only 1 terminal)
    
        for(int j = 0; j < v; ++j) {
            IloExpr totalLinks(env);
            for(int i = j+1; i < v; ++i){
                totalLinks += path[j][i];
            }
        }

        // must be a tree
        IloExpr totalEdges(env);
        IloExpr totalVertex(env);
        for(int i = 0; i < v; ++i)totalVertex+= onAns[i];
        for(int i = 0; i < v; ++i){
            for(int j = i+1; j < v; ++j){
                totalEdges += path[i][j];
            }
        }
        totalVertex-= 1;
        model.add(totalEdges == totalVertex);
    
    //obj function

    IloExpr totalCost(env);
    for(int i = 0; i < v; ++i){
        for(int j = i+1; j < v; ++j){
            totalCost+= path[i][j] * cost[i][j];
        }
    }
    model.add(IloMinimize(env, totalCost));

    // results

    cplex.solve();

    printf("Minimum Cost = %.0lf\n", cplex.getObjValue());

    env.end();
    return 0;
}