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
            if(!p->alpha.count(c)) {
                p->alpha[c] = new TrieNode();
            }
            p = p->alpha[c];
        }
        p->isOk = true;
    }

    bool search(string s) {
        TrieNode *p = head;

        for(char& c : s) {
            if(!p->alpha.count(c)) {
                return false;
            }
            p = p->alpha[c];
        }
        return p->isOk;
    }

    bool startWith(string s) {
        TrieNode *p = head;

        for(char& c : s) {
            if(!p->alpha.count(c)) {
                return false;
            }
            p = p->alpha[c];
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
