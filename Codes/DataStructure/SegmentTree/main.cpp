#include<vector>
#include<iostream>
#include<memory>
#include<functional>
#include<algorithm>

using namespace std;

template<typename T, typename F = plus<T>>
class SegmentTree {
private:
    vector<T>& arr;
    vector<T> tree;

    void buildTree(int left, int right, int node) {
        if(left == right) {
            tree[node] = arr[left];
        } else {
            int mid = (left + right)/2;
            int left_node = node*2 + 1;
            int right_node = node*2 + 2;

            buildTree(left, mid, left_node);
            buildTree(mid+1, right, right_node);
            tree[node] = F()(tree[left_node], tree[right_node]);
        }
    }

    T _query(int left, int right, int node, int l, int r) {
        if(right < l || left > r) {
            return 0;
        } else if(left >= l && right <= r) {
            return tree[node];
        } else {
            int mid = (left + right)/2;
            int left_node = node*2 + 1;
            int right_node = node*2 + 2;
            T oper_left = _query(left, mid, left_node, l, r);
            T oper_right = _query(mid+1, right, right_node, l, r);
            return F()(oper_left, oper_right);
        }
    }

    void _update(int left, int right, int node, int index, T val) {
        if(left == right) {
            arr[left] = val;
            tree[node] = val;
        } else {
            int mid = (left + right)/2;
            int left_node = node*2 + 1;
            int right_node = node*2 + 2;

            if(index <= mid) {
                _update(left, mid, left_node, index, val);
            } else {
                _update(mid+1, right, right_node, index, val);
            }
            tree[node] = F()(tree[left_node], tree[right_node]);
        }
    }

public:
    SegmentTree(vector<T>& a) : arr(a) {
        tree.resize(arr.size()*4, 0);
        buildTree(0, arr.size()-1, 0);
    }

    void update(int index, T val) {
        _update(0, arr.size()-1, 0, index, val);
    }

    T query(int left, int right) {
        return _query(0, arr.size()-1, 0, left, right);
    }
};

template<typename T>
struct Max {
    T operator()(const T& a, const T& b) const {
        return max<T>(a, b);
    }
};

int main() {
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    unique_ptr<SegmentTree<int>> segtree(new SegmentTree<int>(arr));

    cout << "sum of arr[0~3]: " << segtree->query(0, 3) << endl;
    segtree->update(2, 13);
    cout << "sum of arr[2~5]: " << segtree->query(2, 5) << endl;

    vector<int> arr2 = {3, 1, 5, 9, 11, 2};
    unique_ptr<SegmentTree<int, Max<int>>>
               segtree_max(new SegmentTree<int, Max<int>>(arr2));
    cout << "max of arr2[0~3]: " << segtree_max->query(0, 3) << endl;

    return 0;
}
