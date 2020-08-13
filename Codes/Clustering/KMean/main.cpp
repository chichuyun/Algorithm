#include<iostream>
#include<vector>
#include<random>
#include<algorithm>
#include<numeric>

using namespace std;

typedef vector<vector<double>> Vec2;
typedef vector<vector<vector<double>>> Vec3;

default_random_engine e;
uniform_real_distribution<double> engine(-1.0, 1.0);

class KMean {
private:
    int unsigned k, M, N;
    Vec2 nums, cens, cens2;
    Vec3 res;

    double getDistance(vector<double>& p, vector<double>& q) {
        double sum = 0;
        for(unsigned i=0; i<M; ++i) {
            sum += (p[i] - q[i])*(p[i] - q[i]);
        }
        return sum;
    }

    void _clear() {
        res.clear();
        res.resize(k);
    }

    void _loop() {
        unsigned int itype;
        double minV, dis;
        for(unsigned i=0; i<N; ++i) {
            minV = numeric_limits<double>::max();
            for(unsigned j=0; j<k; ++j) {
                dis = getDistance(cens[j], nums[i]);
                if(dis < minV) {
                    minV = dis;
                    itype = j;
                }
            }
            res[itype].push_back(nums[i]);
        }

        cens2 = cens;
        for(unsigned i=0; i<k; ++i) {
            for(unsigned h=0; h<M; ++h) cens[i][h] = 0;
            for(unsigned j=0; j<res[i].size(); ++j) {
                for(unsigned h=0; h<M; ++h) {
                    cens[i][h] += res[i][j][h];
                }
            }
            for(unsigned h=0; h<M; ++h) {
                if(res.size()!=0) cens[i][h] /= static_cast<double>(res[i].size());
            }
        }
    }
public:
    KMean(const unsigned int k, Vec2& nums) : k(k), nums(nums) {
        N = nums.size();
        M = nums[0].size();
        cens.resize(k, vector<double>(M));
        cens2.resize(k, vector<double>(M));
        for(unsigned i=0; i<k; ++i) {
            for(unsigned h=0; h<M; ++h) {
                cens[i][h] = engine(e);
                cens2[i][h] = cens[i][h] + 1.0;
            }
        }
    };

    void fit() {
        while(cens != cens2) {
            _clear();
            _loop();
        }
    }

    Vec3 GetClasses() {
        return res;
    }

    Vec2 GetCenters() {
        return cens;
    }
};

int main() {
    const unsigned int N = 1000;
    const unsigned int M = 2;
    const unsigned int k = 5;

    Vec2 nums(N, vector<double>(M));
    for(unsigned i=0; i<N; ++i) {
        for(unsigned j=0; j<M; ++j) {
            nums[i][j] = engine(e);
        }
    }

    KMean *s = new KMean(k, nums);
    s->fit();
    Vec3 res = s->GetClasses();
    Vec2 cens = s->GetCenters();

    for(unsigned i=0; i<k; ++i) {
        cout << "Class " << i+1 << ": ";
        for(unsigned h=0; h<M; ++h) cout << cens[i][h] << " ";
        cout << endl;
        for(unsigned j=0; j<res[i].size(); ++j) {
            for(unsigned h=0; h<M; ++h) cout << res[i][j][h] << " ";
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}
