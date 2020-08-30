#include<iostream>
#include<vector>
#include<unordered_map>

using namespace std;

struct DNode {
    DNode *prev;
    DNode *next;
    int key, val;
    DNode() {}
    DNode(int& key, int& val) : key(key), val(val) {
        prev = nullptr;
        next = nullptr;
    }
};

class LRUCache {
private:
    int len = 0;
    int N;
    DNode *head;
    DNode *tail;
    unordered_map<int, DNode*> lists;
public:
    LRUCache(int N) : N(N) {
        head = new DNode();
        tail = new DNode();
        head->next = tail;
        tail->prev = head;
    }

    void setCache(int key, int num) {
        DNode *l, *r, *p;
        if(lists.count(key)) {
            p = lists[key];
            l = p->prev;
            r = p->next;
            l->next = r;
            r->prev = l;
            p->val = num;
            --len;
       } else if(len>=N) {
            p = new DNode(key, num);
            l = head->next;
            r = l->next;
            lists.erase(l->key);
            delete l;
            head->next = r;
            r->prev = head;
            --len;
        } else {
            p = new DNode(key, num);
        }
        l = tail->prev;
        l->next = p;
        p->prev = l;
        p->next = tail;
        tail->prev = p;
        lists[key] = p;
        ++len;
    }

    int getCache(int key) {
        int num = -1;
        DNode *l, *r, *p;
        if(lists.count(key)) {
            p = lists[key];
            l = p->prev;
            r = p->next;
            l->next = r;
            r->prev = l;
            num = p->val;
        }
        return num;
    }
};

int main() {
    LRUCache *lru = new LRUCache(3);
    lru->setCache(1, 2);
    lru->setCache(1, 4);
    lru->setCache(2, 3);
    lru->setCache(3, 6);
    lru->setCache(5, 7);
    lru->setCache(2, 8);

    for(int i=0; i<6; ++i) {
        cout << "key: " << i << " value: " << lru->getCache(i) << endl;
    }

    return 0;
}
