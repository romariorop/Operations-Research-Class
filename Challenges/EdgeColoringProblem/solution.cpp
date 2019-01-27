#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include <vector>
#include <utility>

using namespace std;

const int MAX = 510;



int main() {

    IloEnv env;
    IloModel model(env, "Edge Coloring Problem");
    IloCplex cplex(model);
    int colorsSize = 10;
    int v, e, v1, v2;
    scanf("%d %d", &v, &e);
    vector< pair<int, int> > edges;
    vector<int> edgesFromVertex[MAX];
    for(int i = 0; i < e; ++i){ // edges to vertex so i can know when a edge is adjascent to another one
        scanf("%d %d", &v1, &v2);
        v1--;v2--;
        edgesFromVertex[v1].push_back(i);
        edgesFromVertex[v2].push_back(i);
        edges.push_back(make_pair(v1, v2));
    }

    // decision variables

    IloIntVarArray colors(env, e, 1, colorsSize); // the color of each edge


    // restrictions

        //adjascent vertices can not have the same color
    for(int i = 0; i < v; ++i){
        for(int v_ : edgesFromVertex[i]){
            for(int _v : edgesFromVertex[i]){
                if(v_ != _v){
                    model.add(colors[v_] != colors[_v]);
                }
            }
        }
    }

    //obj function

    IloExpr totalCost(env); // total cost of all edges, where color(i) costs (i), and i > 0
    for(int i = 0; i < e; ++i){
        totalCost += colors[i];
    }
    model.add(IloMinimize(env, totalCost));

    // results
    cplex.solve();
    IloNumArray solutions(env, e);
    cplex.getValues(solutions, colors);
    printf("Total Cost = %.0lf\n", cplex.getObjValue());
    for(int i = 0; i < e; ++i)printf("vertex %d  = color %.0lf\n", i+1, solutions[i]);
    env.end();
    return 0;
}