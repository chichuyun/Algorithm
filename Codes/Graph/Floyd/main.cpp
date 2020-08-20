#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<algorithm>
#include<numeric>

#define MAX_NUM 10000

using namespace std;

typedef vector<int> Vec1;
typedef vector<Vec1> Vec2;
typedef vector<double> VecD1;
typedef vector<VecD1> VecD2;
typedef pair<Vec1, double> PairW;
typedef vector<PairW> VecPairW;

class Floyd {
private:
    int N;
    VecPairW lists;
    Vec2 routes;
    VecD2 costs;
    bool flag = true;

    struct _func {
        bool operator()(PairW& p, PairW& q) {
            return p.second > q.second;
        }
    };

    void _adjacency_matrix() {
        costs.resize(N, VecD1(N, MAX_NUM));
        for(auto& l : lists) {
            Vec1 &p = l.first;
            costs[p[0]][p[1]] = l.second;
        }
        for(int i=0; i<N; ++i) costs[i][i] = 0.0;
    }

    Vec1 _route(int i, int j) {
        int k = routes[i][j];
        if(k == -1) {
            if(i != j) return {i, j};
            return {i};
        }

        Vec1 lefts = _route(i, k);
        Vec1 rights = _route(k, j);

        Vec1 res;
        for(int l : lefts) res.push_back(l);
        for(int r : rights) res.push_back(r);

        return res;
    }
public:
    Floyd(const int N, VecPairW& lists) : N(N), lists(lists) {}

    void solve() {
        _adjacency_matrix();

        routes.resize(N, Vec1(N, -1));
        for(int k=0; k<N; ++k) {
            for(int i=0; i<N; ++i) {
                for(int j=0; j<N; ++j) {
                    double cost = costs[i][k] + costs[k][j];
                    if(costs[i][j] > cost) {
                        costs[i][j] = cost;
                        routes[i][j] = k;
                    }
                }
            }
        }

        flag = false;
    }

    VecD1 getLists(int i) {
        if(flag) solve();
        return costs[i];
    }

    double getMinCost(int i, int j) {
        if(flag) solve();
        return costs[i][j];
    }

    Vec1 getRoute(int i, int j) {
        if(flag) solve();
        if(costs[i][j] >= MAX_NUM) return {};
        Vec1 res =  _route(i, j);
        Vec1 route;
        int pre = res[0];
        route.push_back(pre);
        for(int i=1; i<res.size(); ++i) {
            if(res[i]!=pre) route.push_back(res[i]);
            pre = res[i];
        }
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
                      {{2, 7}, -0.34},
                      {{6, 2}, 0.40},
                      {{3, 6}, 0.52},
                      {{6, 0}, 0.58},
                      {{6, 4}, 0.93}};

    Floyd *s = new Floyd(N, lists);

    s->solve();
    for(int i=0; i<N; ++i) {
        double minCost = s->getMinCost(0, i);
        Vec1 route = s->getRoute(0, i);
        if(minCost >= MAX_NUM) {
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
