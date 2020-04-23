#include<iostream>
#include<vector>

using namespace std;

typedef vector<int> vecInt;

vecInt mergesort(vecInt &, int, int);
vecInt merge(vecInt &, vecInt &);

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

    vecInt res = mergesort(lst,0,lst.size()-1);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<res.size();++i) 
        cout<<res[i]<<" ";
    cout<<endl;
}

vecInt mergesort(vecInt &lst, int l, int r) {
    if(r<l) {
        return {};
    } else if(r==l) {
        return {lst[r]};
    }
    int m = (l+r)/2;
    vecInt left  = mergesort(lst, l, m);
    vecInt right = mergesort(lst, m+1, r);
    return merge(left, right);
}

vecInt merge(vecInt& left, vecInt& right) {
    if(left.empty()) return right;
    if(right.empty()) return left;
    vecInt res(left.size()+right.size());
    int i=0, j=0, len1=left.size(), len2=right.size();
    while(i<len1 || j<len2) {
        if(i<len1 && j<len2) {
            if(left[i]<=right[j]) {
                res[i+j] = left[i];
                ++i;
            } else {
                res[i+j] = right[j];
                ++j;
            }
        } else if(i==len1) {
            res[i+j] = right[j];
            ++j;
        } else if(j==len2) {
            res[i+j] = left[i];
           ++i;
        }
    }
    return res;
}
