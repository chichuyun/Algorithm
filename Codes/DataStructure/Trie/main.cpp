#include<iostream>
#include<string>
#include<unordered_map>

using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode*> alpha;
    bool isOk;
    TrieNode() : isOk(false) {}
};

class Trie {
private:
    TrieNode *head;
public:
    Trie() {
        head = new TrieNode();
    }

    void insert(string s) {
        TrieNode *p = head;

        for(char& c : s) {
            if(!p->alpha.count(c-'a')) {
                p->alpha[c-'a'] = new TrieNode();
            }
            p = p->alpha[c-'a'];
        }
        p->isOk = true;
    }

    bool search(string s) {
        TrieNode *p = head;

        for(char& c : s) {
            if(!p->alpha.count(c-'a')) {
                return false;
            }
            p = p->alpha[c-'a'];
        }
        return p->isOk;
    }

    bool startWith(string s) {
        TrieNode *p = head;

        for(char& c : s) {
            if(!p->alpha.count(c-'a')) {
                return false;
            }
            p = p->alpha[c-'a'];
        }
        return true;
    }
};

int main() {
    Trie *trie = new Trie();
    trie->insert("abcd");
    trie->insert("ajfldsf");
    trie->insert("eeee");

    cout << trie->search("abcd") << endl;
    cout << trie->search("abc")  << endl;
    cout << trie->startWith("abc") << endl;

    return 0;
}
