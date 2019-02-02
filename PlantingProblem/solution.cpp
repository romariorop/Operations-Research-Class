#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

struct Farm { 
    double area;
    double water; 
};
struct Cultivation {
    double maxArea;
    double waterPerAcre;
    double profitPerAcre;
};

int main(){

    IloEnv env;
    IloModel model(env, "Planting Problem");
    IloCplex cplex(model);

    int f, c; 
    scanf("%d %d", &f, &c);
    Farm farms[f]; 
    Cultivation cultivations[c];

    for(int i = 0; i < f; ++i) {
        scanf("%lf %lf", &farms[i].area, &farms[i].water);
    }

    for(int i = 0; i < c; ++i) {
        scanf("%lf %lf %lf", &cultivations[i].maxArea, &cultivations[i].waterPerAcre, &cultivations[i].profitPerAcre);
    }
    IloArray<IloNumVarArray> x(env, f);

    for(int i = 0; i < f; ++i) {
        x[i] = IloNumVarArray(env, c, 0, IloInfinity);
    }

  // restrictions
   // prop
      IloExpr baseProportion(env);
      for(int j = 0; j < c; ++j) baseProportion += x[0][j];
      baseProportion /= farms[0].area;
      for(int i = 1; i < f; ++i){
        IloExpr proportion(env);
        for(int j = 0; j < c; ++j) proportion += x[i][j];
        proportion /= farms[i].area;
        model.add(baseProportion == proportion);
      }
    // water
        for(int i = 0; i < f; ++i){
            IloExpr totalWater(env);
            for(int j = 0; j < c; ++j)totalWater+= x[i][j] * cultivations[j].waterPerAcre;
            model.add(totalWater <= farms[i].water);
        }

    // maxArea 
      for(int j = 0; j < c; ++j){
        IloExpr totalArea(env);
        for(int i = 0; i < f; ++i) totalArea+= x[i][j];
        model.add(totalArea <= cultivations[j].maxArea);
      }
    
    // maxFarmArea
      for(int i = 0; i < f; ++i){
        IloExpr area(env);
        for(int j = 0; j < c; ++j) area += x[i][j];
        model.add(area <= farms[i].area);
      }
   
  
  // obj function
    IloExpr profit(env);
    for (int j = 0; j < c; ++j) {
        for (int i = 0; i < f; ++i) {
            profit += x[i][j] * cultivations[j].profitPerAcre;
        }
    }
      
    model.add(IloMaximize(env, profit));

  // results
    cplex.solve();
    printf("maxProfit: %lf\n", cplex.getObjValue());
    IloArray<IloNumArray> solutions(env, f);
    for(int i = 0; i < c; ++i) {
      solutions[i] = IloNumArray(env, c);
      cplex.getValues(solutions[i], x[i]);
    }
    for(int i = 0; i < f; ++i) {
      printf("Farm %d\n", i + 1);
      for(int j = 0; j < c; ++j) printf("\tCultivation %d: %lf\n", j + 1, solutions[i][j]);
    }

  env.end();
  return(0);
}