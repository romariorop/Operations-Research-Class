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
    
    IloArray<IloBoolVarArray> colors(env,e);
    for(int i = 0; i < e; ++i)colors[i] = IloBoolVarArray(env, e);// color[i][j] = true if edges i have a color j


    // restrictions

        // edges must have exactly one color
        for(int i = 0; i < e; ++i){
            IloExpr totalColors(env);
            for(int j = 0; j < v; ++j)totalColors+= colors[i][j];
            model.add(totalColors == 1);
        }

        //adjascent vertices can not have the same color
        for(int i = 0; i < v; ++i){
            for(int v_ : edgesFromVertex[i]){
                for(int _v : edgesFromVertex[i]){
                    if(v_ != _v){
                        for(int c = 0; c < e; ++c){
                            model.add(colors[v_][c] + colors[_v][c] <= 1);
                        }
                        
                    }
                }
            }
        }

    //obj function

    IloExpr totalCost(env); // total cost of all edges, where color(i) costs (i), and i > 0
    for(int i = 0; i < e; ++i){
        for(int j = 0; j < e; ++j){
            totalCost += colors[i][j] * (j+1);
        }
    }
    model.add(IloMinimize(env, totalCost));

    // results
    cplex.solve();
    IloNumArray solutions(env, e);
    printf("Total Cost = %.0lf\n", cplex.getObjValue());
    for(int i = 0; i < e; ++i){
        cplex.getValues(solutions, colors[i]);
        for(int j = 0; j < e; ++j){
            if(solutions[j]){
                printf("vertex %d  = color %d\n", i+1, j+1);
            }
        }
    }
    env.end();
    return 0;
}