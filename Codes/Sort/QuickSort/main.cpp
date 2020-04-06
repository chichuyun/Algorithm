#include<iostream>
#include<vector>
#include<utility>

using namespace std;

typedef vector<int> vecInt;

void quicksort(vecInt &, int, int);
int partition(vecInt &, int, int);

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

    quicksort(lst,0,lst.size()-1);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<lst.size();++i) 
        cout<<lst[i]<<" ";
    cout<<endl;
}

void quicksort(vecInt &lst, int l, int r) {
    if(l>=r) return;
    int m = partition(lst, l, r);
    quicksort(lst,l,m-1);
    quicksort(lst,m+1,r);
}

int partition(vecInt &lst, int l, int r) {
    int v;
    int i, j;
    v = lst[l];
    while(true) {
        for(i=l;i<r && lst[i]<=v;++i);
        for(j=r;j>l && lst[j]>v;--j);
        if(i<j) swap(lst[i],lst[j]); else break;
    }
    swap(lst[l],lst[j]);
    return j;
}
