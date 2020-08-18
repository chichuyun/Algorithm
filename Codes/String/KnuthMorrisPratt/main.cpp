#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

typedef vector<int> Vec1;

class KMP {
private:
    string str;
    string pat;
    int N;
    Vec1 nexts;

    void _generate_next() {
        N = pat.size();
        nexts.clear();
        nexts.resize(N);

        nexts[0] = -1;
        int k = -1, i = 0;
        while(i < N) {
            if(k == -1 || pat[i] == pat[k]) {
                ++k;
                ++i;
                nexts[i] = k;
		    } else {
                k = nexts[k];
		    }
        }
    }
public:
    KMP(string str) : str(str) {}

    int match(string pat) {
        this->pat = pat;
        _generate_next();

        int i = 0, j = 0;
        while(i < str.size() && j < N) {
            if(j == -1 || str[i]==pat[j]) {
                ++i;
                ++j;
            } else {
                j = nexts[j];
            }
        }
        if(j == N) return i - N;
        return -1;
    }
};

int main() {
    string str = "abcxabcdabxabcdabcdabcy";

    KMP *k = new KMP(str);
    cout << "String:  " << str << endl;
    string str1 = "abcababcabca";
    cout << "str1: " << str1 << " " << k->match(str1) << endl;
    string str2 = "abcx";
    cout << "str2: " << str2 << " " << k->match(str2) << endl;
    string str3 = "abcdabcy";
    cout << "str3: " << str3 << " " << k->match(str3) << endl;

    return 0;
}
