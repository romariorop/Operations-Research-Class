#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include <vector>
#include <cmath>
using namespace std;

struct print{
    int cover;
    int sheet;
    int size;
    int duration;
};

int main() {

    IloEnv env;
    IloModel patternProblem(env, "Pattern Problem");
    IloCplex cplex(patternProblem);

    int n;
    scanf("%d", &n);
    vector<print>prints;
    for(int i = 0; i < n; ++i){
        print p;
        scanf("%d %d %d %d", &p.size, &p.sheet, &p.cover, &p.duration);
        prints.push_back(p);
    }
    int amount[3], timeLimit = 400;
    int sheetCost = 5, coverCost = 3;
    int tinprice = 50;
    amount[1] = 200;
    amount[2] = 50;

    IloIntVar y(env, 0, IloInfinity);
    IloIntVarArray x(env, n, 0, IloInfinity);


    //restrictions

    IloExpr totalTime(env);
    for(int i = 0; i < n; ++i){
        totalTime += x[i] * prints[i].duration;
    }
    patternProblem.add(totalTime <= timeLimit);

    IloExpr size1(env);
    IloExpr size2(env);
    for(int i = 0; i < n; ++i){
        if(prints[i].size == 1)size1+=x[i];
        else size2+= x[i];
    }
    patternProblem.add(size1 <= amount[1]);
    patternProblem.add(size2 <= amount[2]);
    

    //obj Function

    IloExpr profit;
    IloExpr totalSheets(env);
    IloExpr totalCovers (env);
    IloExpr totalCovers2 (env);
    for(int i = 0; i < n; ++i){
        totalSheets+= x[i] * prints[i].sheet;
        totalCovers+= x[i] * prints[i].cover;
    }
    patternProblem.add(y <= totalSheets);
    patternProblem.add(y <= totalCovers / 2);
    totalCovers2 = totalCovers;
    totalCovers2 /= 2;
    profit = y * tinprice - sheetCost * (totalSheets - y) - coverCost * (totalCovers - (y*2));
    patternProblem.add(IloMaximize(env, profit));
    


    cplex.solve();


    printf("Maximum Profit = %lf\n", cplex.getObjValue());
    IloInt solution = cplex.getValue(y);
    printf("%d tins\n", solution);
    IloNumArray amounts(env);
    cplex.getValues(amounts, x);
    for(int i = 0; i < n; ++i){
        printf("%.0lf of type %d\n", fabs(amounts[i]), i+1);
    }
    env.end();

    return 0;
}