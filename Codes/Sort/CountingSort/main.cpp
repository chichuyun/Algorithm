#include<iostream>
#include<vector>

using namespace std;

typedef vector<int> vecInt;

vecInt countingsort(vecInt &);

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

    vecInt res = countingsort(lst);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<res.size();++i) 
        cout<<res[i]<<" ";
    cout<<endl;
}

vecInt countingsort(vecInt &lst) {
    int num = 0;
    for(int n : lst) num = max(num, n); // find the max number.
    vecInt counts(num+1,0);
    for(int n : lst) ++counts[n];   // counting numbers
    for(int i=1;i<=num;++i) counts[i] += counts[i-1];  // accumulation
    vecInt res(lst.size(),0);
    for(int i=lst.size()-1;i>=0;--i) {
        res[counts[lst[i]]-1] = lst[i];
        --counts[lst[i]];
    }
    return res;
}
