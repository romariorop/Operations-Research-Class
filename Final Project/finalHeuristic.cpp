#include <bits/stdc++.h>

using namespace std;
#define PRINT 1
const int MAX = 200;
int grau[MAX], routerCandidate[MAX], eloCandidate[MAX], strongEloCandidate[MAX];
int v, e, t, l, r, from, to;
vector<pair<int, int> >edges;
set<int>terminals;
vector<int>eCand, seCand, rCand, others;
void updateRank(int u) {
    grau[u]++;
    if(grau[u] == 1)eloCandidate[u] = 1;
    else if(grau[u] == 2){
        eloCandidate[u] = 0;
        strongEloCandidate[u] = 1;
    } else if(grau[u] == 3){
        strongEloCandidate[u] = 0;
        routerCandidate[u] = 1;
    }
}

void updateCandidates() {
    for(int i = 0; i < e; ++i) {
        int v1 = edges[i].first;
        int v2 = edges[i].second;
        if(terminals.count(v1))updateRank(v2);
        if(terminals.count(v2))updateRank(v1);
    }
    for(int i = 0; i < e; ++i){
        int v1 = edges[i].first;
        int v2 = edges[i].second;
        if(routerCandidate[v1] || routerCandidate[v2])rCand.push_back(i);
        else if(strongEloCandidate[v1] || strongEloCandidate[v2])seCand.push_back(i);
        else if(eloCandidate[v1] || eloCandidate[v2])eCand.push_back(i);
        else others.push_back(i);
    }
}
void shuffleAll() {
    random_shuffle(eCand.begin(), eCand.end());
    random_shuffle(seCand.begin(), seCand.end());
    random_shuffle(rCand.begin(), rCand.end());
    random_shuffle(others.begin(), others.end());
}

vector<int> runGrasp() {
    int runs = 30;
    bool optimal = false;
    vector<int>bestSolution;
    int score = -10000;
    while(runs && !optimal){
        shuffleAll();


        --runs;
    }
}
int main() {
 
    // v = Vertex / e == Edges // t == Terminals // l = linkers // r routers
    
    scanf("%d %d", &v, &e);
    for(int i = 0; i < e; ++i){
        scanf("%d %d", &from, &to);
        edges.push_back({from, to});
    }
    scanf("%d", &t);
    for(int i = 0; i < t; ++i) {
        int x;
        scanf("%d", &x);
        terminals.insert(x);
    }
    scanf("%d %d", &l, &r);
    updateCandidates();
    if(PRINT) {
        puts("Router Candidates");
        for(int i = 1; i <= v; ++i)if(routerCandidate[i])printf("%d ", i);
        puts("\nStrong Elo Candidates");
        for(int i = 1; i <= v; ++i)if(strongEloCandidate[i])printf("%d ", i);
        puts("\nElo Candidates");
        for(int i = 1; i <= v; ++i)if(eloCandidate[i])printf("%d ", i);
        puts("\nOthers");
        for(int i = 1; i <= v; ++i)if(!routerCandidate[i] && !eloCandidate[i] && !strongEloCandidate[i])printf("%d ", i);
        puts("");
    }
    vector<int> solution = runGrasp();
    

    return 0;
}