#include <stdio.h>
#include <vector>
#include <ilcplex/ilocplex.h>
using namespace std;

const int MAX = 50;

int main() {

    IloEnv env;
    IloModel model(env, "Vertex Cover Problem");
    IloCplex cplex(model);

    vector<int>adj[MAX];

    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; ++i){
        int m, k;
        scanf("%d", &m);
        for(int j = 0; j < m; ++j){
            scanf("%d", &k);
            adj[i].push_back(k);
        }
    }

    //variables
    IloBoolVarArray x(env, n); // true:vertex choosed

    //restrictions
    
    for(int i = 0; i < n; ++i){
        IloExpr total(env);
        total+= x[i];
        for(int j : adj[i]){
            total+= x[j];
        }
        model.add(total >= 1);
    }

    //obj function

    IloExpr total(env);
    for(int i = 0; i < n; ++i) {
        total+= x[i];
    }
    model.add(IloMinimize(env, total));

    cplex.solve();

    printf("%.0lf\n", cplex.getObjValue());

    IloNumArray solution(env, n);
    cplex.getValues(solution, x);

    for(int i = 0; i < n; ++i){
        if(solution[i])printf("vertex %d choosed\n", i);
    }

    env.end();
    return 0;
}