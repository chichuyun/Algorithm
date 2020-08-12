#include<utility>
#include<vector>
#include<iostream>

using namespace std;

class UnionFind {
private:
    int N;
    int count;
    vector<int> ranks, parents;
    vector<pair<int,int>> lists;

    void _union(int i, int j) {
        int root_i = _find(i);
        int root_j = _find(j);
        if(root_i == root_j) return;
        if(ranks[root_i] > ranks[root_j]) {
            parents[root_j] = root_i;
            ranks[root_i] += ranks[root_j];
        } else {
            parents[root_i] = root_j;
            ranks[root_j] += ranks[root_i];
        }
        --count;
    }

    int _find(int i) {
        if(parents[i] != i) {
            parents[i] = _find(parents[i]);
        }

        return parents[i];
    }
public:
    UnionFind(vector<pair<int,int>> lists, int N) {
        this->N = N;
        this->lists = lists;
        count = N;
        ranks.resize(N, 1);
        parents.resize(N);
        for(int i=0; i<N; ++i) parents[i] = i;
    }

    int solve() {
        for(auto l : lists) {
            _union(l.first, l.second);
        }

        return count;
    }
};

int main() {

    vector<pair<int,int>> lists = {{1,2},
                                   {2,3},
                                   {1,3},
                                   {2,4},
                                   {6,8},
                                   {9,7},
                                   {0,3},
                                   {7,8}};

    UnionFind *u = new UnionFind(lists, 10);

    cout << u->solve() << endl;

    return 0;
}
