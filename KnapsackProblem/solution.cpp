#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

const int MAX = 1e5;

int main() {

    IloEnv env;
    IloModel knapsackProblem(env, "Knapsack Problem");
    IloCplex cplex(knapsackProblem);

    int weight[MAX];
    int value[MAX];

    int n, cap;
    scanf("%d %d", &n, &cap);
    for(int i = 0; i < n; ++i)scanf("%d", &weight[i]);
    for(int i = 0; i < n; ++i)scanf("%d", &value[i]);

    IloBoolVarArray x(env, n);

    //restructions
    IloExpr exprSum(env);
    for(int i = 0; i < n; ++i)exprSum+= x[i] * weight[i];
    knapsackProblem.add(exprSum <=  cap);

    // objective function
    IloExpr exprObj(env);
    for(int i = 0; i < n; ++i)exprObj+= x[i] * value[i];
    knapsackProblem.add(IloMaximize(env, exprObj));

    cplex.solve();

    IloNumArray solutions(env, n);
    cplex.getValues(solutions, x);

    printf("Maximum value = %lf\n", cplex.getObjValue());
    for(int i = 0; i < n; ++i)printf("Item %d : %d\n", i, solutions[i] ? 1 : 0);
    env.end();






    return 0;
}