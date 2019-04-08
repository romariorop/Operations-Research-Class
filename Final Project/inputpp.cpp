#include <bits/stdc++.h>
#define PRINT 0
using namespace std;
const int MAX = 200;
vector<pair<int, int> >edges;
vector<int> vertexToEdge[MAX];
set<int>terminals;
int connected[MAX][MAX];
int grau[MAX];
int edgeDeleted[MAX], nodeDeleted[MAX];
int deletedEdges;

int main() {
    
    // v = Vertex / e == Edges // t == Terminals // l = linkers // r routers
    int v, e, t;
    int l, r;
    int from, to;
    int temp;
    scanf("%d %d", &v, &e);
    memset(edgeDeleted, 0, sizeof(edgeDeleted));
    memset(nodeDeleted, 0, sizeof (nodeDeleted));
    for(int i = 0; i < e; ++i){
        scanf("%d %d", &from, &to);
        edges.push_back({from, to});
        grau[from]++;
        grau[to]++;
        connected[from][to] = connected[to][from] = 1;
        vertexToEdge[from].push_back(i);
        vertexToEdge[to].push_back(i);
    }
    scanf("%d", &t);
    for(int i = 0; i < t; ++i) {
        scanf("%d", &temp);
        terminals.insert(temp);
    }
    scanf("%d %d", &l, &r);
    priority_queue< pair<int, int> > pq;
    for(int i = 1; i <= v; ++i){
        pq.push({-grau[i], i});
    }
    while(!pq.empty()){
        int x = pq.top().second;
        pq.pop();
        if(terminals.count(x) || nodeDeleted[x])continue;
        if(grau[x] == 1){
            if(PRINT)printf("node %d deleted by the rule 1\n", x);
            nodeDeleted[x] = 1;
            for(int id : vertexToEdge[x]){
                if(!edgeDeleted[id]){
                    int y = edges[id].first == x ? edges[id].second : edges[id].first;
                    grau[x]--;
                    grau[y]--;
                    pq.push({-grau[y], y});
                    edgeDeleted[id] = 1;
                    deletedEdges++;
                }
            }
        } else if(grau[x] == 2) {
            int left = -1, right;
            for(int id : vertexToEdge[x]) {
                if(!edgeDeleted[id]){
                    int y = edges[id].first == x ? edges[id].second : edges[id].first;
                    if(left == -1)left = y;
                    else right = y;
                }
            }
            if(!connected[left][right])continue;
            if(PRINT)printf("node %d deleted by the rule 2\n", x);
            nodeDeleted[x] = 1;
            for(int id : vertexToEdge[x]){
                if(!edgeDeleted[id]){
                    int y = edges[id].first == x ? edges[id].second : edges[id].first;
                    grau[x]--;
                    grau[y]--;
                    pq.push({-grau[y], y});
                    edgeDeleted[id] = 1;
                    deletedEdges++;
                }
            }
        } else {
            break;
        }
    }
    printf("%d %d\n", v, e-deletedEdges);
    for(int i = 0; i < e; ++i){
        if(!edgeDeleted[i]){
            printf("%d %d\n", edges[i].first, edges[i].second);
        }
    }
    printf("%d\n", t);
    for(int ter : terminals){
        printf("%d\n", ter);
    }
    printf("%d %d\n", l, r);

    return 0;
}