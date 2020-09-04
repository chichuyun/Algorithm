#include<vector>
#include<iostream>
#include<algorithm>

using namespace std;

class Hungarian {
private:
    vector<int> vis;
    vector<bool> mems;
    vector<vector<bool>> adjs;
    int M, N;
    int count;

    bool _match(int i) {
        for(int j=0; j<N; ++j) {
            if(adjs[i][j] && mems[j]) {
                mems[j] = false;
                if(vis[j] == -1 || _match(vis[j])) {
                    vis[j] = i;
                    return true;
                }
            }
        }
        return false;
    }
public:
    Hungarian(vector<vector<bool>> adjs) : adjs(adjs) {
        M = adjs.size();
        N = adjs[0].size();
        count = 0;
        mems.resize(N);
        vis.resize(N, -1);
    };

    int solve() {
        for(int i=0; i<M; ++i) {
            for(int j=0; j<N; ++j) mems[j] = true;
            if(_match(i)) ++count;
        }
        return count;
    }
};

int main() {
    vector<vector<bool>> adjs = {{1, 0, 1},
                                 {1, 1, 1},
                                 {1, 0, 1}};

    Hungarian *h = new Hungarian(adjs);
    cout << h->solve() << endl;

    return 0;
}
