#include <stdio.h>
#include <string>
#include <ilcplex/ilocplex.h>
using namespace std;



int main() {
    IloEnv env;
    IloModel inksProblem(env, "Inks Problem");
    IloCplex cplex(inksProblem);

    IloIntVarArray SR(env, 4, 0, IloInfinity); // SolA SolB SEC COR
    IloIntVarArray SN(env, 4, 0, IloInfinity); // SolA SolB SEC COR
    double price[] = {1.5, 1, 4, 6};
    string names[] = {"SolA", "SolB", "SEC", "COR"};
    //restrictions
    IloExpr litersSumSR(env);
    IloExpr litersProportionSRsec(env);
    IloExpr litersProportionSRcor(env);
    IloExpr litersSumSN(env);
    IloExpr litersProportionSNsec(env);
    IloExpr litersProportionSNcor(env);
    for(int i = 0; i < 4; ++i){
        litersSumSR+=SR[i];
        litersSumSN+=SN[i];
    }
    inksProblem.add(litersSumSR == 1000);
    inksProblem.add(litersSumSN == 250);

    litersProportionSRsec = (SR[0]*0.3 + SR[1]*0.6 + SR[2])/1000;
    litersProportionSRcor = (SR[0]*0.7 + SR[1]*0.4 + SR[3])/1000;

    inksProblem.add(litersProportionSRsec >= 0.25);
    inksProblem.add(litersProportionSRcor >= 0.5);

    litersProportionSNsec = (SN[0]*0.3 + SN[1]*0.6 + SN[2])/250;
    litersProportionSNcor = (SN[0]*0.7 + SN[1]*0.4 + SN[3])/250;

    inksProblem.add(litersProportionSNsec >=0.2);
    inksProblem.add(litersProportionSNcor >= 0.5);

    //objective function

    IloExpr waste(env);
    for(int i = 0; i < 4; ++i){
        waste += SR[i] * price[i];
        waste += SN[i] * price[i];
    }

    inksProblem.add(IloMinimize(env, waste));



    cplex.solve();
    printf("Minimum Waste = %lf R$\n", cplex.getObjValue());
    IloNumArray SRsolutions(env, 4);
    IloNumArray SNsolutions(env, 4);
    cplex.getValues(SRsolutions, SR);
    cplex.getValues(SNsolutions, SN);
    
    printf("SR Amounts:\n");
    for(int i = 0; i < 4; ++i){
        printf("%s -> %lf \n", names[i].c_str(), fabs(SRsolutions[i]));
    }
    printf("SN Amounts:\n");
    for(int i = 0; i < 4; ++i){
        printf("%s -> %lf \n", names[i].c_str(), fabs(SNsolutions[i]));
    }
    
    env.end();
    return 0;
}