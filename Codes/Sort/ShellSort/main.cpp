#include<iostream>
#include<vector>
#include<utility>
using namespace std;

typedef vector<int> vecInt;

void shellsort(vecInt &);

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

   shellsort(lst);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<lst.size();++i) 
        cout<<lst[i]<<" ";
    cout<<endl;
}

void shellsort(vecInt &lst) {
     int step = lst.size()-1;
     while(step) {
        for(int i=0;i<lst.size();i+=step) {
            int j = i - step, num = lst[i];
            while(j>=0 && lst[j]>num) {
                swap(lst[j],lst[j+step]);
                j -= step;
            }
        }
        step = step >> 1;
    }
}
