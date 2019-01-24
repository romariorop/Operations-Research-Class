#include <stdio.h>
#include <ilcplex/ilocplex.h>

const int MAX  = 50;

int main() {
    IloEnv env;
    IloModel facilitiesProblem(env, "Facilities Problem");
    IloCplex cplex(facilitiesProblem);

    int centers, instalation[MAX], cost[MAX][MAX], clients;
    scanf("%d %d", &centers, &clients);
    for(int i = 0; i < centers; ++i){
        scanf("%d", &instalation[i]);
    }
    for(int i = 0; i < centers; ++i){
        for(int j = 0; j < clients; ++j){
            scanf("%d", &cost[i][j]);
        }
    }

    IloBoolVarArray  x(env, centers);
    IloArray<IloBoolVarArray> y(env, centers);
    for(int i = 0; i < centers; ++i)y[i] = IloBoolVarArray(env, clients);

    //restrictions

    for(int i = 0; i < clients; ++i){
        IloExpr total(env);
        for(int j = 0; j < centers; ++j){
            total+= y[j][i];
        }
        facilitiesProblem.add(total == 1);
    }

    for(int i = 0; i < centers; ++i){
        IloExpr total(env);
        for(int j = 0; j < clients; ++j){
            total += y[i][j];
        }
        facilitiesProblem.add(total <= x[i] * total);
    }
    

    //obj function

    IloExpr totalCost(env);
    for(int i = 0; i < centers; ++i){
        totalCost+= x[i] * instalation[i]; 
        for(int j = 0; j < clients; ++j) {
            totalCost += cost[i][j] * y[i][j];
        }
    }


    facilitiesProblem.add(IloMinimize(env, totalCost));

    cplex.solve();

    printf("Minimum Waste = %lf R$\n", cplex.getObjValue());

    env.end();
    return 0;
}