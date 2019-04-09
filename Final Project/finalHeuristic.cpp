#include <bits/stdc++.h>

using namespace std;
#define PRINT 1
const int MAX = 200;
int root[MAX], weight[MAX];

int getRoot(int x){
    return root[x] = (root[x] == x) ? x : getRoot(root[x]);
}

void merge(int a, int b) {
    a = getRoot(a);
    b = getRoot(b);
    if(weight[a] < weight[b]){
        root[b] = a;
        weight[a] += weight[b];
    } else {
        root[a] = b;
        weight[b] += weight[a];
    }
}

bool sameTree(int a, int b){
    return getRoot(a) == getRoot(b);
}

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

void resetDsu() {
    for(int i = 1; i <= v; ++i)root[i] = i;
}

int dfs(int current, vector<vector<int>> &adj, set<int> &toRemove , vector<int> &depth, int parent = -1) {
    int total = terminals.count(current);
    for(int i = 0; i < adj[current].size(); ++i) {
        int j = adj[current][i];
        if(parent == j)continue;
        depth[j] = depth[i]+1;
        total += dfs(j, adj, toRemove, depth, current);
    }
    if(total == 0){
        toRemove.insert(current);
    }
    return total;
}


vector<int> removeNonTerminalLeafs(vector<int> original) {
    vector< vector<int> > adj(v+1);
    for(int i = 1; i <=v; ++i)adj[i] = vector<int>();
    for(int i = 0; i < original.size(); ++i){
        int a = edges[original[i]].first;
        int b = edges[original[i]].second;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    int root = -1;
    for(int i = 1; i<=v && root == -1; ++i)if(terminals.count(i)){
        root = i;
    }
    set<int>toRemove;
    vector<int>depth(v+1);
    depth[root] = 0;
    dfs(root, adj, toRemove, depth);
    vector<int>ans;
    for(int i = 0; i < original.size(); ++i){
        int a = edges[original[i]].first;
        int b = edges[original[i]].second;
        if(toRemove.count(a) || toRemove.count(b))continue;
        ans.push_back(original[i]);
    }
    return ans;
}

vector<int>getSolution(int forbidden = -1) {
    vector<int>solution;
    resetDsu();
    int take[] = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    int eId, eLim, seId, seLim, rId, rLim, otId, otLim, edgeId;
    eLim = eCand.size();
    seLim = seCand.size();
    rLim = rCand.size();
    otLim = others.size();
    eId = seId = rId = otId = 0;
    while(eId < eLim || seId < seLim || rId < rLim || otId < otLim) {
        random_shuffle(take, take + 10);
        int opt = take[0];
        edgeId = -1;
        if(opt == 1) {
            if(otId == otLim)continue;
            edgeId = others[otId++];
        } else if(opt == 2){
            if(eId == eLim)continue;
            edgeId = eCand[eId++];
        } else if(opt == 3) {
            if(seId == seLim)continue;
            edgeId = seCand[seId++];
        } else if(opt == 4) {
            if(rId == rLim)continue;
            edgeId = rCand[rId++];
        }
        if(edgeId != forbidden) {
            int v1 = edges[edgeId].first;
            int v2 = edges[edgeId].second;
            if(sameTree(v1, v2))continue;
            merge(v1, v2);
            solution.push_back(edgeId);
        }
    }
    solution = removeNonTerminalLeafs(solution);
    return solution;
}

int getPenalty(vector<int> sol) { // todo: add new penalty for the number of edges in solution
    int deg[MAX];
    for(int edgeId: sol){
        int v1 = edges[edgeId].first;
        int v2 = edges[edgeId].second;
        deg[v1]++;
        deg[v2]++;
    }
    int totalR = 0;
    int totalE = 0;
    for(int i = 1; i <=v; ++i) {
        if(deg[i] == 2)totalE++;
        else if(deg[i] > 2)totalR++;
    }
    int penalty = 0;
    if(totalR > r)penalty += 2*(totalR - r);
    if(totalE > l)penalty += totalE - l;
    for(int ter : terminals) {
        for(int ter2 : terminals) {
            if(!sameTree(ter, ter2))penalty+=1000;
        }
    }
    return penalty;
}

vector<int> runGrasp() {
    int runs = 50;
    bool optimal = false;
    vector<int>bestSolution;
    int penalty = 100000;
    
    while(runs && !optimal){
        shuffleAll();
        vector<int>currentSol = getSolution();
        int currentPenalty = getPenalty(currentSol);
        if(currentPenalty < penalty){
            bestSolution = currentSol;
            penalty = currentPenalty;
        }
        for(int i = 0; i < currentSol.size();++i) {
            vector<int>tempSol = getSolution(currentSol[i]);
            int tempPenalty = getPenalty(tempSol);
            if(tempPenalty < penalty){
                bestSolution = tempSol;
                penalty = tempPenalty;
            }
        }
        if(penalty == 0)optimal = true;
        --runs;
    }

    return bestSolution;
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
    puts("Edges:");
    for(int id : solution) {
        printf("%d-%d\n", edges[id].first, edges[id].second);
    }

    return 0;
}