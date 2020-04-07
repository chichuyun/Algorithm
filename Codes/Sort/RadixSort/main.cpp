#include<iostream>
#include<vector>

using namespace std;

typedef vector<int> vecInt;

vecInt radixsort(vecInt &);

int main() {
    vecInt lst;
    int in, num;
    
    cout<<"num: "<<endl;
    cin>>num;
    
    cout<<"Input nums: "<<num<<endl;
    for(int i=0;i<num;++i) {
        cin>>in;
        lst.push_back(in);
    }
    cout<<"Raw array: "<<endl;
    for(int i=0;i<lst.size();++i) 
        cout<<lst[i]<<" ";
    cout<<endl;

    vecInt res = radixsort(lst);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<res.size();++i) 
        cout<<res[i]<<" ";
    cout<<endl;
}

vecInt radixsort(vecInt &lst) {
    int num = 0;
    for(int n : lst) num = max(num, n); // find the max number.
    int base = 0, ten = 1;
    while(num) {
        ++base;
        num /= 10;
    }

    vecInt res(lst.size(),0), counts(10,0);
    while(base--) {
        for(int &n : counts) n = 0;
        for(int n : lst) ++counts[n/ten%10];   // counting numbers
        for(int i=1;i<10;++i) counts[i] += counts[i-1];  // accumulation
        cout<<"counts:  "<<endl;
        for(int n : counts) cout<<n<<' ';
        cout<<endl;
        for(int i=lst.size()-1;i>=0;--i) {
            int m = lst[i]/ten%10;
            res[counts[m]-1] = lst[i];
            --counts[m];
        }
        for(int n : res) cout<<n<<' ';
        cout<<endl;
        lst = res;
        ten *= 10;
    }
    return res;
}
