#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;


int main() {
    IloEnv env;
    IloModel transportationProblem(env, "Transportation Problem");
    IloCplex cplex(transportationProblem);

    int amount[] = {120, 80, 80};
    int need[] = {150, 70, 60};
    int cost[3][3] = {
        {8, 5, 6},
        {15, 10, 12},
        {3, 9, 10}
    };

    IloArray<IloIntVarArray> x(env, 3);
    for(int i = 0; i < 3; ++i)x[i] = IloIntVarArray(env, 3, 0, IloInfinity);

    //restrictions

    for(int i = 0; i < 3; ++i){
        IloExpr from(env);
        for(int j = 0; j < 3; ++j){
            from+=x[i][j];
        }
        transportationProblem.add(from <= amount[i]);
    }

    for(int i = 0; i < 3; ++i){
        IloExpr to(env);
        for(int j = 0; j < 3; ++j){
            to+= x[j][i];
        }
        transportationProblem.add(to == need[i]);
    }


    //objective func


    IloExpr total(env);
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            total+= cost[i][j] * x[i][j];
        }
    }
    transportationProblem.add(IloMinimize(env, total));

    cplex.solve();

    printf("Minimum Waste  = %lf R$\n", cplex.getObjValue());

    IloArray<IloNumArray> amounts(env, 3);
    for(int i = 0; i < 3; ++i){
        printf("Amount from Factory %d : \n", i+1);
        amounts[i] = IloNumArray(env, 3);
        cplex.getValues(amounts[i], x[i]);
        for(int j = 0; j < 3; ++j){
            printf("To Storehouse %d : %lf\n", j+1, fabs(amounts[i][j]));
        }
    }
  





    return 0;
}