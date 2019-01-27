#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include <vector>

using namespace std;
const int MAX = 50;
int flow[MAX][MAX];
int maxIn[MAX];
int main() {
    
    IloEnv env;
    IloModel model(env, "Steiner Problem");
    IloCplex cplex(model);

    int v, e, v1, v2, w, sink, source;

    scanf("%d %d", &v, &e);
    for(int i = 0; i < e; ++i){
        scanf("%d %d %d", &v1, &v2, &w);
        --v1, --v2;
        flow[v1][v2] = w;
        maxIn[v2]+= w;
    }
    scanf("%d %d", &source, &sink);
    --source, --sink;
    // decision variables
    IloIntVarArray flowIn(env, v, 0, IloInfinity);
    IloIntVarArray flowOut(env, v, 0, IloInfinity);


    // restrictions

        //flow conservation
        for(int i = 0; i < v; ++i){
            if(i != source && i != sink){
                model.add(flowIn[i] == flowOut[i]);
            }
        }
        //flow restriction
        for(int i = 0; i < v; ++i){
            model.add(flowIn[i] <= maxIn[i]);
        }

    // obj Function
    model.add(IloMaximize(env, flowIn[sink]));

    //results
    cplex.solve();
    printf("Max Gás FLow = %.0lf\n", cplex.getObjValue());



    env.end();
    return 0;
}