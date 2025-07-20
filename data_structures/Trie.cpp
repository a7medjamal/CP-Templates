struct Trie {
    struct Node {
        Node *child[26];
        int prefix, end, sz; 
        Node() {
            prefix = end = sz = 0;
            memset(child, 0, sizeof(child));
        }
    };
    Node *root;
    Trie() { root = new Node(); }
    void insert(string &s) { // insert a new word 
        Node* cur = root;
        for (auto it : s) {
            int idx = it - 'a';
            if (cur->child[idx] == nullptr) {
                cur->child[idx] = new Node();
            }
            cur = cur->child[idx];
            cur->prefix++;
            if (cur->sz == 0 or s.size() < cur->sz) cur->sz = s.size();
        }
        cur->end++;
    }
    int count(string& s) { // count how many times this word exists
        Node* cur = root;
        for (auto it : s) {
            int idx = it - 'a';
            if (cur->child[idx] == nullptr) {
                return 0;
            }
            cur = cur->child[idx];
        }
        return cur->end;
    }
};
