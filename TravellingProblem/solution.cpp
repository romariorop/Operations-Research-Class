#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

const int MAX = 500;

int main() {
    
    IloEnv env;
    IloModel model(env, "Travelling Salesman Problem");
    IloCplex cplex(model);

    int cost[MAX][MAX];
    int n;
    
    scanf("%d", &n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            scanf("%d", &cost[i][j]);
        }
    }

    // variables

    IloArray<IloBoolVarArray> path(env, n); // path[i][j] = true if there is a path between i and j
    for(int i = 0; i < n; ++i) path[i] = IloBoolVarArray(env, n);

    // restrictions

        // only 1 incoming edge
    for(int i = 0; i < n; ++i){
        IloExpr totalIncoming(env);
        model.add(path[i][i] == 0);
        for(int j = 0; j < n; ++j){
            totalIncoming+= path[j][i];
        }
        model.add(totalIncoming == 1);
    }

        // only 1 exit edge

    for(int i = 0; i < n; ++i){
        IloExpr totalExit(env);
        for(int j = 0; j < n; ++j){
            totalExit+= path[i][j];
        }
        model.add(totalExit == 1);
    }


    // obj function

    IloExpr totalCost(env);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j){
            totalCost += path[i][j] * cost[i][j];
        }
    }
    model.add(IloMinimize(env, totalCost));

    // results

    cplex.solve();
    printf("Total Cost = %.0lf\n", cplex.getObjValue());

    printf("Solution Matrix\n");
    for(int i = 0; i < n; ++i){
        IloNumArray sol(env, n);
        cplex.getValues(sol, path[i]);
        for(int j = 0; j < n; ++j){
            printf("%.0lf ", sol[j]);
        }
        printf("\n");
    }

    env.end();
    return 0;
}