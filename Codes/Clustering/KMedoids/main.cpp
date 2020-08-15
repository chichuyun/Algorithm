#include<iostream>
#include<vector>
#include<random>
#include<unordered_set>
#include<algorithm>
#include<numeric>

using namespace std;

typedef vector<vector<double>> Vec2;
typedef vector<vector<vector<double>>> Vec3;

default_random_engine e;
uniform_real_distribution<double> engine(-1.0, 1.0);

class KMedoids {
private:
    unsigned int k, M, N;
    Vec2 nums, cens, cens2;
    Vec3 res;

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
                dis = _get_distance(cens[j], nums[i]);
                if(dis < minV) {
                    minV = dis;
                    itype = j;
                }
            }
            res[itype].push_back(nums[i]);
        }

        cens2 = cens;
        for(unsigned i=0; i<k; ++i) {
            for(unsigned j=0; j<res[i].size(); ++j) {
                minV = numeric_limits<double>::max();
                dis = _get_distances(res[i][j], i);
                if(dis < minV) {
                    minV = dis;
                    cens[i] = res[i][j];
                }
            }
        }
    }
public:
    KMedoids(const unsigned int k, Vec2& nums) : k(k), nums(nums) {
        N = nums.size();
        M = nums[0].size();
        cens.resize(k, vector<double>(M));
        cens2.resize(k, vector<double>(M));
        uniform_int_distribution<unsigned int> inte(0, N-1);
        unordered_set<unsigned int> medoids;
        for(unsigned i=0; i<k; ++i) {
            int ran_int;
            do {
                ran_int = inte(e);
            } while(medoids.count(ran_int));
            cens[i] = nums[ran_int];
            medoids.insert(i);
        }
    };

    void fit() {
        while(cens != cens2) {
            _clear();
            _loop();
        }
    }

    Vec3 getClasses() {
        return res;
    }

    Vec2 getCenters() {
        return cens;
    }
};

int main() {
    const unsigned int N = 10000;
    const unsigned int M = 2;
    const unsigned int k = 5;

    Vec2 nums(N, vector<double>(M));
    for(unsigned i=0; i<N; ++i) {
        for(unsigned j=0; j<M; ++j) {
            nums[i][j] = engine(e);
        }
    }

    KMedoids *s = new KMedoids(k, nums);
    s->fit();
    Vec3 res = s->getClasses();
    Vec2 cens = s->getCenters();

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
