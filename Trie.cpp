#include <memory>
#include <string>
using namespace std;

class TrieNode {
public:
    unique_ptr<unique_ptr<TrieNode>[]> children;

    TrieNode() : children(nullptr) {}
};

class Trie {
private:
    unique_ptr<TrieNode> head;

    void allocateChildren(TrieNode* node) {
        if (!node->children) {
            node->children = make_unique<unique_ptr<TrieNode>[]>(27);
        }
    }

public:
    Trie() : head(make_unique<TrieNode>()) {}

    void insert(const string& word) {
        TrieNode* cur = head.get();

        for (char c : word) {
            int idx = c - 'a';

            allocateChildren(cur);

            if (!cur->children[idx]) {
                cur->children[idx] = make_unique<TrieNode>();
            }

            cur = cur->children[idx].get();
        }

        allocateChildren(cur);

        if (!cur->children[26]) {
            cur->children[26] = make_unique<TrieNode>();
        }
    }

    bool search(const string& word) const {
        const TrieNode* cur = head.get();

        for (char c : word) {
            int idx = c - 'a';

            if (!cur->children || !cur->children[idx]) {
                return false;
            }

            cur = cur->children[idx].get();
        }

        return cur->children &&
               cur->children[26] != nullptr;
    }

    bool startsWith(const string& prefix) const {
        const TrieNode* cur = head.get();

        for (char c : prefix) {
            int idx = c - 'a';

            if (!cur->children || !cur->children[idx]) {
                return false;
            }

            cur = cur->children[idx].get();
        }

        return true;
    }
};
