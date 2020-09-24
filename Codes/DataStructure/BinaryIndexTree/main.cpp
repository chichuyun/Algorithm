#include<vector>
#include<iostream>
#include<memory>
#include<functional>
#include<algorithm>

using namespace std;

template<typename T>
class BinaryIndexTree {
private:
    vector<T> tree;  // first index is 1.
    vector<T>& nums;
    int len;

    int _lowbit(int x) {
        return x & (-x);
    }

    void _add_sum(int i, T& val) {
        while(i <= len) {
            tree[i] += val;
            i += _lowbit(i);
        }
    }

    void _buildTree(vector<T>& nums) {
        tree.resize(len+1, 0);
        for(int i=1; i<=len; ++i) {
            _add_sum(i, nums[i-1]);
        }
    }

    T _sum(int i) {
        T res = 0;
        while(i > 0) {
            res += tree[i];
            i -= _lowbit(i);
        }
        return res;
    }

public:
    BinaryIndexTree(vector<T>& ns) : nums(ns) {
        len = nums.size();
        _buildTree(nums);
    }

    void update(int i, T val) {
        val = val - nums[i];
        nums[i] += val;
        _add_sum(i+1, val);
    }

    T query(int i, int j) {
        return _sum(j+1) - _sum(i);
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
    unique_ptr<BinaryIndexTree<int>> bitree(new BinaryIndexTree<int>(arr));

    cout << "sum of arr[0~3]: " << bitree->query(0, 3) << endl;
    bitree->update(2, 13);
    cout << "sum of arr[2~5]: " << bitree->query(2, 5) << endl;

    return 0;
}
