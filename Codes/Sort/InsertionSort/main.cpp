#include<iostream>
#include<vector>
#include<utility>
using namespace std;

typedef vector<int> vecInt;

void insertionsort(vecInt &);

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

   insertionsort(lst);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<lst.size();++i) 
        cout<<lst[i]<<" ";
    cout<<endl;
}

void insertionsort(vecInt &lst) {
    for(int i=0;i<lst.size();++i) {
        int j = i - 1, num = lst[i];
        while(j>=0 && lst[j]>num) {
            swap(lst[j],lst[j+1]);
            --j;
        }
    }
}
