#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX = 50;
int adj[MAX][MAX];
int adj2[MAX][MAX];
int main() {

    IloEnv env;
    IloModel model(env, "Maximum Balanced Biclique Problem");
    IloCplex cplex(model);

    int n, m, e;
    int u, v;
    
    scanf("%d %d %d", &n, &m, &e);
    for(int i = 0; i < e; ++i){
        scanf("%d %d", &u, &v);
        --u,--v;
        adj[u][v] = 1;
        adj2[v][u] = 1;
    }

    // decision variables

    IloBoolVarArray v1(env, n); // vertex from g1 that is on answer
    IloBoolVarArray v2(env, m); // vertex from g2 that is on answer

    //restrictions

        // balanced
        IloExpr countV1(env);
        for(int i = 0; i < n; ++i)countV1+= v1[i];
        IloExpr countV2(env);
        for(int i = 0; i < m; ++i)countV2+= v2[i];
        model.add(countV1 == countV2);

        // clique restriction
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j){
                if(!adj[i][j]){
                    model.add(v1[i]+v2[j] <= 1);
                }
            }
        }

    // obj function

        //maximize the number of vertex
        IloExpr totalVertex(env);
        for(int i = 0; i < n; ++i)totalVertex+= v1[i];
        for(int i = 0; i < m; ++i)totalVertex+= v2[i];
        model.add(IloMaximize(env, totalVertex));


    // results

    cplex.solve();
    printf("Total Vertexes %.0lf \n", cplex.getObjValue());

    IloNumArray g1(env);
    IloNumArray g2(env);
    cplex.getValues(g1, v1);
    cplex.getValues(g2, v2);
    printf("Vertex from g1 -> ");
    for(int i = 0; i < n; ++i){
        if(g1[i])printf("%d ", i+1);
    }
    printf("\nVertex from g2 -> ");
    for(int i = 0; i < m; ++i){
        if(g2[i])printf("%d ", i+1);
    }

    return 0;
}