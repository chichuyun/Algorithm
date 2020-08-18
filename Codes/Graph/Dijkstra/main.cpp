#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<algorithm>
#include<numeric>

using namespace std;

typedef vector<int> Vec1;
typedef vector<Vec1> Vec2;
typedef vector<double> VecD1;
typedef vector<VecD1> VecD2;
typedef pair<Vec1, double> PairW;
typedef vector<PairW> VecPairW;

const double DOUBLE_MAX = numeric_limits<double>::max();

class Dijkstra {
private:
    int N;
    VecPairW lists;
    Vec2 routes;
    VecD2 costs;
    vector<VecPairW> adjs;

    struct _func {
        bool operator()(PairW& p, PairW& q) {
            return p.second > q.second;
        }
    };

    void _adjacency_list() {
        adjs.resize(N);
        for(auto& l : lists) {
            Vec1 p = l.first;
            adjs[p[0]].push_back(l);
        }
    }
public:
    Dijkstra(const int N, VecPairW& lists) : N(N), lists(lists) {
        costs.resize(N);
        routes.resize(N);
    }

    void solve(int i) {
        _adjacency_list();
        costs[i].resize(N, DOUBLE_MAX);
        costs[i][i] = 0;
        routes[i].resize(N, -1);
        routes[i][i] = i;

        Vec1 vised(N, 0);
        vised[i] = 1;

        priority_queue<PairW, VecPairW, _func> edges;
        for(PairW& adj : adjs[i]) edges.push(adj);
        while(!edges.empty()) {
            PairW edge = edges.top();
            Vec1 &id = edge.first;
            edges.pop();
            if(vised[id[1]]) continue; // if this node has been already visited
            vised[id[1]] = 1;
            routes[i][id[1]] = id[0];
            costs[i][id[1]] = edge.second;

            for(PairW& adj : adjs[id[1]]) {
                edges.emplace(adj.first, adj.second+edge.second); // relaxation
            }
        }
    }

    void solveAll() {
        for(int i=0; i<N; ++i) solve(i);
    }

    VecD1 getLists(int i) {
        if(costs[i].empty()) solve(i);
        return costs[i];
    }

    double getMinCost(int i, int j) {
        if(costs[i].empty()) solve(i);
        return costs[i][j];
    }

    Vec1 getRoute(int i, int j) {
        if(costs[i].empty()) solve(i);
        Vec1 route;
        if(routes[i][j]== -1) return {};  // unreachable
        route.push_back(j);
        while(routes[i][j]!=i) {
            route.push_back(routes[i][j]);
            j = routes[i][j];
        }
        route.push_back(i);
        reverse(route.begin(), route.end());
        return route;
    }
};

int main() {
    const int N = 8;

    VecPairW lists = {{{4, 5}, 0.35},
                      {{5, 4}, 0.35},
                      {{4, 7}, 0.37},
                      {{5, 7}, 0.28},
                      {{7, 5}, 0.28},
                      {{5, 1}, 0.32},
                      {{0, 4}, 0.38},
                      {{0, 2}, 0.26},
                      {{7, 3}, 0.39},
                      {{1, 3}, 0.29},
                      {{2, 7}, 0.34},
                      {{6, 2}, 0.40},
                      {{3, 6}, 0.52},
                      {{6, 0}, 0.58},
                      {{6, 4}, 0.93}};

    Dijkstra *s = new Dijkstra(N, lists);

    s->solve(0);
    for(int i=0; i<N; ++i) {
        double minCost = s->getMinCost(0, i);
        Vec1 route = s->getRoute(0, i);
        if(minCost==DOUBLE_MAX) {
            cout << "0 -> " << i << " minCost: " << "Inf." << endl;
            cout << "Unreachable!" << endl;
        } else {
            cout << "0 -> " << i << " minCost: " << minCost << endl;
            cout << route[0];
            for(int i=1; i<route.size(); ++i) {
                cout << " -> " << route[i];
            }
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}
