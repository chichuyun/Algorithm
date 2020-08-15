#include<iostream>
#include<vector>
#include<random>
#include<algorithm>
#include<numeric>

using namespace std;

typedef vector<double> Vec1;
typedef vector<vector<double>> Vec2;
typedef vector<vector<vector<double>>> Vec3;

default_random_engine e;
uniform_real_distribution<double> engine(-1.0, 1.0);

class KNearestNeighbor {
private:
    unsigned int k, M, N;
    Vec3 knns, res;
    Vec2 nums;

    double _get_distance(vector<double>& p, vector<double>& q) {
        double sum = 0;
        for(unsigned i=0; i<M; ++i) {
            sum += (p[i] - q[i])*(p[i] - q[i]);
        }
        return sum;
    }

    double _get_distances(vector<double>& p, int j) {
        double sum = 0;

        for(unsigned i=0; i<res[j].size(); ++i) {
            sum += _get_distance(p, res[j][i]);
        }
        for(unsigned i=0; i<knns[j].size(); ++i) {
            sum += _get_distance(p, knns[j][i]);
        }
        return sum;
    }

    void _clustering() {
        unsigned int itype;
        double minV, dis;
        for(unsigned i=0; i<N; ++i) {
            minV = numeric_limits<double>::max();
            for(unsigned j=0; j<k; ++j) {
                dis = _get_distances(nums[i], j);
                if(dis < minV) {
                    itype = j;
                    minV = dis;
                }
            }
            res[itype].push_back(nums[i]);
        }
    }
public:
    KNearestNeighbor(const unsigned int k, Vec3& knns, Vec2& nums) : k(k), knns(knns), nums(nums) {
        N = nums.size();
        M = nums[0].size();
        res.resize(k);
    };

    void fit() {
        _clustering();
    }

    Vec3 getClasses() {
        return res;
    }
};

int main() {
    const unsigned int N = 10000;
    const unsigned int N2 = 100;
    const unsigned int M = 2;
    const unsigned int k = 5;

    Vec3 knns(k, Vec2(N2, Vec1(M)));
    for(unsigned i=0; i<k; ++i) {
        for(unsigned j=0; j<N2; ++j) {
            for(unsigned h=0; h<M; ++h) {
                knns[i][j][k] = engine(e);
            }
        }
    }

    Vec2 nums(N, vector<double>(M));
    for(unsigned i=0; i<N; ++i) {
        for(unsigned j=0; j<M; ++j) {
            nums[i][j] = engine(e);
        }
    }

    KNearestNeighbor *s = new KNearestNeighbor(k, knns, nums);
    s->fit();
    Vec3 res = s->getClasses();

    for(unsigned i=0; i<k; ++i) {
        cout << "Class " << i+1 << ": ";
        for(unsigned j=0; j<res[i].size(); ++j) {
            for(unsigned h=0; h<M; ++h) cout << res[i][j][h] << " ";
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}
