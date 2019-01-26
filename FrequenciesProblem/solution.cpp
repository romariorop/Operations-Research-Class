#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

const int MAX = 500;

int main() {

    IloEnv env;
    IloModel frequenciesProblem(env, "Frequencies Problem");
    IloCplex cplex(frequenciesProblem);
    int adjMatrix[MAX][MAX];
    int n, frequencies;
    scanf("%d %d", &n, &frequencies);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j){
            scanf("%d", &adjMatrix[i][j]);
        }
    }

    IloBoolVarArray x(env, frequencies);
    IloArray<IloBoolVarArray>y (env, n);
    for(int i = 0; i < n; ++i)y[i] = IloBoolVarArray(env, frequencies);


    // restrictions

    for(int i = 0; i < n; ++i){
        for(int j = i+1; j < n; ++j){
            if(adjMatrix[i][j]){
                for(int k = 0; k < frequencies; ++k){
                    frequenciesProblem.add(y[i][k] + y[j][k] <= 1);
                }
            }
        }
        IloExpr totalFrequencies(env);
        for(int j = 0; j < frequencies; ++j){
            totalFrequencies+= y[i][j];
            frequenciesProblem.add(y[i][j] <= x[j]);
        }
        frequenciesProblem.add(totalFrequencies == 1);
    }


    // obj function

    IloExpr total(env);
    for(int i = 0; i < frequencies; ++i)total+= x[i];
    frequenciesProblem.add(IloMinimize(env, total));


    cplex.solve();

    printf("%.0lf frequencies used\n", cplex.getObjValue());

    IloArray<IloNumArray> freqs(env, n);
    for(int i = 0; i < n; ++i){
        freqs[i] = IloNumArray(env, frequencies);
        cplex.getValues(freqs[i], y[i]);
    }
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < frequencies; ++j){
            if(freqs[i][j])printf("Node %d uses Freq %d\n", i+1, j+1);
        }
    }

    env.end();
    return 0;
}