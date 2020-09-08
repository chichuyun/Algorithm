#include<iostream>

using namespace std;

int mutliMod(int base, int a, int mod) {
    int ans = 1;
    while(a != 0) {
        if(a & 1) {
            ans = (ans*base)%mod;
        }
        base = (base*base)%mod;
        a >>= 1;
    }
    return ans;
}

int main() {
    int base = 5;
    int a = 1000;
    int mod = 7;

    // (base^a)%mod
    cout << mutliMod(base, a, mod) << endl;

    return 0;
}
