#include<iostream>
#include<vector>
#include<utility>
using namespace std;

typedef vector<int> vecInt;

void bubblesort(vecInt &);

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

   bubblesort(lst);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<lst.size();++i) 
        cout<<lst[i]<<" ";
    cout<<endl;
}

void bubblesort(vecInt &lst) {
    for(int i=0;i<lst.size();++i) {
        for(int j=1;j<lst.size();++j) {
            if(lst[j-1]>lst[j]) swap(lst[j-1],lst[j]);
        }
    }
}
