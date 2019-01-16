#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;


int main() {
    IloEnv env;
    IloModel feedProblem(env, "Feed Problem");
    IloCplex cplex(feedProblem);

    IloIntVarArray x(env, 2, 0, IloInfinity); // x[0] = AMGS , x[1] = RE

    //restrictions
    IloExpr expr(env);
    expr = x[0] + 4*x[1];
    feedProblem.add(expr <= 10000);
    expr = 5*x[0] + 2*x[1];
    feedProblem.add(expr <= 30000); 

    //objective func
    expr = 11*x[0] + 12*x[1];
    feedProblem.add(IloMaximize(env, expr));


    cplex.solve();
    IloNumArray solutions(env, 2);
    
    printf("Maximum Profit = %lf R$\n", cplex.getObjValue());
    cplex.getValues(solutions, x);
    printf("AMGS Quantity : %lf\n", solutions[0]);
    printf("RE Quantity : %lf\n", solutions[1]);





    return 0;
}