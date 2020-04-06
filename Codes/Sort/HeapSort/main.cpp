#include<iostream>
#include<vector>
#include<utility>
using namespace std;

typedef vector<int> vecInt;

inline int parent(int i) {return (i-1)>>1;}
inline int left(int i) {return i+i+1;}
inline int right(int i) {return i+i+2;}

vecInt heapsort(vecInt &);
void buildminheap(vecInt &);
void heapify(vecInt &, int, int);
int heappop(vecInt &, int);

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

   vecInt res = heapsort(lst);
    cout<<"Sored array: "<<endl;
    for(int i=0;i<res.size();++i)
        cout<<res[i]<<" ";
    cout<<endl;
}

vecInt heapsort(vecInt& heap) {
    vecInt res(heap.size());
    
    buildminheap(heap);
    for(int i=0;i<heap.size();++i) {
        res[i] = heappop(heap, heap.size()-i);
    }
    return res;
}

void buildminheap(vecInt& heap) {
    for(int i=heap.size()/2;i>=0;--i) {
        heapify(heap, i, heap.size());
    }
}

void heapify(vecInt& heap, int i, int heapsize) {
    int l = left(i), r = right(i);
    int lowest = i;
    if(l<heapsize && heap[l] < heap[i]) lowest = l;
    if(r<heapsize && heap[r] < heap[lowest]) lowest = r;
    if(lowest != i) {
        swap(heap[lowest], heap[i]);
        heapify(heap, lowest, heapsize);
    }
}

int heappop(vecInt& heap, int heapsize) {
    int num = heap[0];
    if(heapsize>1) {
        swap(heap[0], heap[heapsize-1]);
        heapify(heap, 0, heapsize-1);
    }
    return num;
}
