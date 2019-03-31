#include <bits/stdc++.h>

using namespace std;
const int MAX = 100;

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

vector<int> buildValidSubject(vector<pair<int,int> >edges, int v) {
    vector<int>population;
    for(int i = 1; i <= v; ++i)root[i] = i;
    for(int i = 0; i < edges.size(); ++i) {
        int a = edges[i].first;
        int b = edges[i].second;
        if(sameTree(a, b))continue;
        merge(a, b);
        population.push_back(i);
    }
    return population;
}

int dfs(int current, vector<vector<int>> &adj, set<int> &terminals, set<int> &toRemove , vector<int> &depth, int parent = -1) {
    int total = terminals.count(current);
    for(int i = 0; i < adj[current].size(); ++i) {
        int j = adj[current][i];
        if(parent == j)continue;
        depth[j] = depth[i]+1;
        total += dfs(j, adj, terminals, toRemove, depth, current);
    }
    if(total == 0){
        toRemove.insert(current);
    }
    return total;
}

vector<int> removeNonTerminalLeafs(vector<int> original, vector<pair<int, int> > &edges, set<int> &terminals, int v) {
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
    dfs(root, adj, terminals, toRemove, depth);
    vector<int>ans;
    for(int i = 0; i < original.size(); ++i){
        int a = edges[original[i]].first;
        int b = edges[original[i]].second;
        if(toRemove.count(a) || toRemove.count(b))continue;
        ans.push_back(original[i]);
    }
    return ans;
}

int main() {
    vector<pair<int, int> >edges;
    set<int>terminals;
    // v = Vertex / e == Edges // t == Terminals // l = linkers // r routers
    int v, e, t;
    int l, r;
    int from, to;
    scanf("%d %d", &v, &e);
    for(int i = 0; i < e; ++i){
        scanf("%d %d", &from, &to);
        edges.push_back({from, to});
    }
    scanf("%d", &t);
    for(int i = 0; i < t; ++i) {
        int temp;
        scanf("%d", &temp);
        terminals.insert(temp);
    }
    scanf("%d %d", &l, &r);
    vector<int> valid = buildValidSubject(edges, v);
    for(int i = 0; i < valid.size(); ++i){
        printf("%d %d\n", edges[valid[i]].first, edges[valid[i]].second);
    }
    vector<int> valid2 = removeNonTerminalLeafs(valid, edges, terminals, v);
    puts("---------------------------------------------------------------");
    for(int i = 0; i < valid2.size(); ++i){
        printf("%d %d\n", edges[valid2[i]].first, edges[valid2[i]].second);
    }
    return 0;
}