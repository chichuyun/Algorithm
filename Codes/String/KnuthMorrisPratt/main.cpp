#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

typedef vector<int> Vec1;

class KMP {
private:
    int N;
    string str;
    string pat;
    Vec1 nexts;

    void _generate_next() {
        N = pat.size();
        nexts.clear();
        nexts.resize(N);

        nexts[0] = 0;
        int pre = 0;
        for(int i=1; i<N; ++i) {
            if(pat[i]==pat[pre]) {
                ++pre;
                nexts[i] = nexts[i-1] + 1;
            } else if(pat[i]==pat[0]) {
                pre = 1;
                nexts[i] = 1;
            } else {
                pre = 0;
                nexts[i] = 0;
            }
        }

        for(int i=N-1; i>0; --i) nexts[i] = nexts[i-1];
        nexts[0] = -1;
    }
public:
    KMP(string str) : str(str) {}

    int match(string pat) {
        this->pat = pat;
        _generate_next();

        int i = 0, j = 0;
        while(i < str.size()) {
            if(str[i]==pat[j]) {
                ++i;
                ++j;
            } else {
                j = nexts[j];
                if(j < 0) {
                    ++i;
                    j = 0;
                }
            }
            if(j == N) return i - N;
        }

        return -1;
    }
};

int main() {
    string str = "abcxabcdabxabcdabcdabcy";

    KMP *k = new KMP(str);
    cout << "String:  " << str << endl;
    string str1 = "abcababcxabab";
    cout << "str1: " << str1 << " " << k->match(str1) << endl;
    string str2 = "abcx";
    cout << "str2: " << str2 << " " << k->match(str2) << endl;
    string str3 = "abcdabcy";
    cout << "str3: " << str3 << " " << k->match(str3) << endl;

    return 0;
}
