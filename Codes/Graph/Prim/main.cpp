#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<algorithm>

using namespace std;

typedef pair<vector<int>, double> PairW;
typedef vector<PairW> VecPairW;
typedef vector<int> Vec1;

class Prim {
private:
    int N;
    VecPairW lists;
    VecPairW res;
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
            adjs[p[0]].emplace_back(vector<int>{p[0], p[1]}, l.second);
            adjs[p[1]].emplace_back(vector<int>{p[1], p[0]}, l.second);
        }
    }
public:
    Prim(const int N, VecPairW& lists) : N(N), lists(lists) {}

    void solve() {
        _adjacency_list();

        vector<int> points(N, 0);  // Hash Table
        points[0] = 1;
        int count = 1;
        priority_queue<PairW, VecPairW, _func> nodes;  // Min Heap
        for(auto& adj : adjs[0]) nodes.push(adj);
        while(count < N) {
            PairW l = nodes.top();
            nodes.pop();
            Vec1 p = l.first;

            if(points[p[1]]==0) {
                res.push_back(l);
                ++count;
                points[p[1]] = 1;
                for(auto& adj : adjs[p[1]]) {
                    if(points[adj.first[1]]==0) {
                        nodes.push(adj);
                    }
                }
            }
        }
    }

    VecPairW getLists() {
        return res;
    }

    double getMinCost() {
        if(res.empty()) solve();

        double cost = 0;
        for(auto& p : res) cost += p.second;
        return cost;
    }
};

int main() {
    const int N = 8;

    VecPairW lists = {{{4, 5}, 0.35},
                      {{4, 7}, 0.37},
                      {{5, 7}, 0.28},
                      {{0, 7}, 0.16},
                      {{1, 5}, 0.32},
                      {{0, 4}, 0.38},
                      {{2, 3}, 0.17},
                      {{1, 7}, 0.19},
                      {{0, 2}, 0.26},
                      {{1, 2}, 0.36},
                      {{1, 3}, 0.29},
                      {{2, 7}, 0.34},
                      {{6, 2}, 0.40},
                      {{3, 6}, 0.52},
                      {{6, 0}, 0.58},
                      {{6, 4}, 0.93}};

    Prim *s = new Prim(N, lists);

    s->solve();
    VecPairW res = s->getLists();
    double minCost = s->getMinCost();
    cout << "Min Cost: " << minCost << endl;
    for(int i=0; i<res.size(); ++i) {
        cout << res[i].first[0] << " " << res[i].first[1] << endl;
    }
    cout << endl;

    return 0;
}
