#include <bits/stdc++.h>

using namespace std;


int main() {
    vector<pair<int, int> >edges;
    vector<int>terminals;
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
    terminals = vector<int>(t);
    for(int i = 0; i < t; ++i) {
        scanf("%d", &terminals[i]);
    }
    scanf("%d %d", &l, &r);
    

    return 0;
}