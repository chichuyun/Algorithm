#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<algorithm>

using namespace std;

typedef pair<vector<int>, double> PairW;
typedef vector<PairW> VecPairW;
typedef vector<int> Vec1;
typedef vector<vector<int>> Vec2;

class Kruskal {
private:
    int N;
    vector<int> ranks, parents;
    VecPairW lists;
    VecPairW res;

    static bool func(PairW& p, PairW& q) {
        return p.second < q.second;
    }

    void _sort() {
        sort(lists.begin(), lists.end(), func);
    }

    void _union(PairW& p) {
        int root_i = _find(p.first[0]);
        int root_j = _find(p.first[1]);
        if(root_i == root_j) return;
        if(ranks[root_i] > ranks[root_j]) {
            parents[root_j] = root_i;
            ranks[root_i] += ranks[root_j];
        } else {
            parents[root_i] = root_j;
            ranks[root_j] += ranks[root_i];
        }
    }

    int _find(int i) {
        if(parents[i] != i) {
            parents[i] = _find(parents[i]);
        }
        return parents[i];
    }

    bool _haveSameRoot(PairW& p) {
        return _find(p.first[0]) == _find(p.first[1]);
    }
public:
    Kruskal(const int N, VecPairW& lists) : N(N), lists(lists) {
        ranks.resize(N, 1);
        parents.resize(N);
        for(int i=0; i<N; ++i) parents[i] = i;
    }

    void solve() {
        _sort();

        for(int i=0; i<lists.size(); ++i) {
            if(!_haveSameRoot(lists[i])) {
                _union(lists[i]);
                res.push_back(lists[i]);
            }
        }
    }

    VecPairW getLists() {
        return res;
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

    Kruskal *s = new Kruskal(N, lists);

    s->solve();
    VecPairW res = s->getLists();
    for(int i=0; i<res.size(); ++i) {
        cout << res[i].first[0] << " " << res[i].first[1] << endl;
    }
    cout << endl;

    return 0;
}
