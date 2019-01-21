#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    IloEnv env;
    IloModel cliqueProblem(env, "Clique Problem");
    IloCplex cplex(cliqueProblem);

    vector<vector<int>> adjMatrix;
    int n, lig;
    scanf("%d", &n);

    for(int i = 0; i < n; ++i){
        vector<int> v;
        for(int j = 0; j < n; ++j){
            scanf("%d", &lig);
            v.push_back(lig);
        }
        adjMatrix.push_back(v);
    }

    IloBoolVarArray  x(env, n);

    //restrictions

    IloExpr checking(env);
    checking += 1;
    
    for(int i = 0; i < n; ++i){
        for(int j = i+1; j < n; ++j){
            if(!adjMatrix[i][j]){
                cliqueProblem.add(x[i] + x[j] <= 1);
            }
        }
    }

    //obj function

    IloExpr total(env);
    for(int i = 0; i < n; ++i)total+= x[i];
    cliqueProblem.add(IloMaximize(env, total));

    cplex.solve();

    printf("Maximum Clique = %lf\n", cplex.getObjValue());
    IloNumArray belong(env);
    cplex.getValues(belong, x);
    printf("Clique vertex ->");
    for(int i = 0; i < n; ++i)if(belong[i])printf(" %d", i+1);



    




    return 0;
}