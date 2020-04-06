#include<iostream>
#include<vector>
#include<utility>
using namespace std;

typedef vector<int> vecInt;

void selectionsort(vecInt &);

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

   selectionsort(lst);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<lst.size();++i) 
        cout<<lst[i]<<" ";
    cout<<endl;
}

void selectionsort(vecInt &lst) {
    for(int i=0;i<lst.size();++i) {
        int minv = lst[i], k = i;
        for(int j=i+1;j<lst.size();++j) {
            if(lst[j]<minv) {
                minv = lst[j];
                k = j;
            }
        }
        if(k!=i) swap(lst[i],lst[k]);
    }
}
