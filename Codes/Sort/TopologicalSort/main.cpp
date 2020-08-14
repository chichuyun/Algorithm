#include<iostream>
#include<vector>
#include<queue>

using namespace std;

typedef vector<int> Vec1;
typedef vector<vector<int>> Vec2;

class TopologicalSort {
private:
    int N;
    Vec2 adj_list;
    Vec1 indegrees;
    Vec1 res;

    void _in_degree() {
        for(int i=0; i<N; ++i) {
            for(int j : adj_list[i]) {
                indegrees[j]++;
            }
        }
    }
public:
    TopologicalSort(Vec2& adj_list) : adj_list(adj_list) {
        N = (int)adj_list.size();
        indegrees.resize(N, 0);
    }

    bool sort() {
        _in_degree();

        queue<int> nodes;
        for(int i=0; i<N; ++i) {
            if(indegrees[i]==0) nodes.push(i);
        }

        while(!nodes.empty()) {
            int n = nodes.size();
            while(n--) {
                int num = nodes.front();
                nodes.pop();
                res.push_back(num);
                for(int j : adj_list[num]) {
                    indegrees[j]--;
                    if(indegrees[j]==0) nodes.push(j);
                }
            }
        }
        if(res.size()==N) {
            return true;
        } else {
            cout << "Error: This garph has a circle!" << endl;
            return false;
        }
    }

    Vec1 getSortedList() {
        return res;
    }
};

int main() {
    Vec2 adj_list = {{1,2}, // 0
                     {3},   // 1
                     {4,6}, // 2
                     {5},   // 3
                     {5,6}, // 4
                     {7},   // 5
                     {8},   // 6
                     {8},   // 7
                     {9},   // 8
                     {}};   // 9
    TopologicalSort *s = new TopologicalSort(adj_list);

    if(s->sort()) {
        Vec1 res = s->getSortedList();
        cout << "sorted list: " << endl;
        for(int i=0; i<adj_list.size(); ++i) {
            cout << res[i] << " ";
        }
        cout << endl;
    }

    return 0;
}
