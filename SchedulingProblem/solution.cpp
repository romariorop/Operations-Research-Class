#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {

    IloEnv env;
    IloModel schedulingProblem(env, "Scheduling Problem");
    IloCplex cplex(schedulingProblem);



    int required[7];
    for(int i = 0; i < 7; ++i)scanf("%d", &required[i]);


    IloIntVarArray x(env, 7, 0, IloInfinity);

    //restrictions

    for(int i = 0; i < 7; ++i){
        IloExpr total(env);
        for(int j = i; j < i+5; ++j){
            total+= x[j%7];
        }
        int day = (i+4)%7;
        schedulingProblem.add(total >= required[day]);
    }

    //obj function

    IloExpr totalNurses(env);
    for(int i = 0; i < 7; ++i){
        totalNurses += x[i];
    }
    schedulingProblem.add(IloMinimize(env, totalNurses));

    cplex.solve();

    printf("Minimum Nurses Required = %.0lf\n", cplex.getObjValue());
    IloNumArray solutions(env, 7);
    cplex.getValues(solutions, x);
    for(int id = 0; id < 7; ++id)printf("%.0lf Nurses required in day %d\n", fabs(solutions[id]), id+1);
    env.end();
    return 0;
}