#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    IloEnv env;
    IloModel dietProblem(env, "Diet Problem");
    IloCplex cplex(dietProblem);
    int A[] = {1, 0, 2, 2, 1, 2};
    int C[] = {0, 1, 3, 1, 3, 2};
    int price[] = {35, 30, 60, 50, 27, 22};

    IloIntVarArray x(env, 6, 0, IloInfinity);


    //restrictions
    IloExpr exprA(env);
    for(int i = 0; i < 6; ++i)exprA+= A[i] * x[i];
    dietProblem.add(exprA >= 9);
    IloExpr exprC(env);
    for(int i = 0; i < 6; ++i)exprC+= C[i] * x[i];
    dietProblem.add(exprC >= 19);
    
    // objective function
    IloExpr exprObj(env);
    for(int i = 0; i < 6; ++i)exprObj+= x[i] * price[i];
    dietProblem.add(IloMinimize(env, exprObj));

    cplex.solve();

    IloNumArray solutions(env, 6);

    printf("Minimum Cost = %lf R$\n", cplex.getObjValue());
    cplex.getValues(solutions, x);
    for(int i = 0; i < 6; ++i)printf("Amount of item %d : %lf\n", i+1, abs(solutions[i]));
    env.end();

    return 0;
}