/*
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <algorithm>
#include <string>

int_fast32_t max_len = 1000001;

struct Node {
    explicit Node(int_fast32_t l = 0, int_fast32_t r = 0, int_fast32_t parent = -1)
        : l(l), r(r), parent(parent) {
    }

    int_fast32_t l, r, parent, link = -1;
    std::unordered_map<char, int_fast32_t> children;

    int_fast32_t Len() const {
        return r - l;
    }

    int_fast32_t &Get(char c) {
        if (!children.count(c)) {
            children[c] = -1;
        }
        return children[c];
    }
};

class Trie {

public:
    explicit Trie() {
        nodes_.emplace_back();
    }
    void AddSymbol(char symbol);

    void GetAns(std::string *operation, std::string *stri);

private:
    std::string input_str_;

    std::vector<Node> nodes_;

    std::pair<int_fast32_t, int_fast32_t> ptr_ = std::make_pair(0, 0);

    std::pair<int_fast32_t, int_fast32_t> Go(std::pair<int_fast32_t, int_fast32_t> st,
                                             int_fast32_t l, int_fast32_t r);

    std::pair<int_fast32_t, int_fast32_t> GoOnes(std::pair<int_fast32_t, int_fast32_t> st,
                                                 int_fast32_t l, const char *s);

    bool IsContains(std::string *stri);

    int_fast32_t Split(std::pair<int32_t, int32_t> st);

    int_fast32_t GetLink(int_fast32_t v);
};

int_fast32_t Trie::GetLink(int_fast32_t v) {
    if (nodes_[v].link != -1) {
        return nodes_[v].link;
    }
    if (nodes_[v].parent == -1) {
        return 0;
    }
    int_fast32_t to = GetLink(nodes_[v].parent);

    return nodes_[v].link = Split(Go(std::make_pair(to, nodes_[to].Len()),
                                     nodes_[v].l + (nodes_[v].parent == 0), nodes_[v].r));
}

int_fast32_t Trie::Split(std::pair<int32_t, int32_t> st) {
    if (st.second == nodes_[st.first].Len()) {
        return st.first;
    }

    if (st.second == 0) {
        return nodes_[st.first].parent;
    }

    Node v = nodes_[st.first];

    int_fast32_t id = nodes_.size();
    nodes_.emplace_back();
    nodes_[id] = Node(v.l, v.l + st.second, v.parent);

    nodes_[v.parent].Get(input_str_[v.l]) = id;
    nodes_[id].Get(input_str_[v.l + st.second]) = st.first;
    nodes_[st.first].parent = id;
    nodes_[st.first].l += st.second;
    return id;
}

void Trie::AddSymbol(char symbol) {
    int_fast32_t pos = input_str_.size();
    input_str_ += symbol;
    bool fl = true;
    while (fl) {
        std::pair<int_fast32_t, int_fast32_t> nptr = Go(ptr_, pos, pos + 1);
        if (nptr.first != -1) {
            ptr_ = nptr;
            return;
        }
        int_fast32_t mid = Split(ptr_);

        int_fast32_t leaf = nodes_.size();
        nodes_.emplace_back();

        nodes_[leaf] = Node(pos, max_len, mid);
        nodes_[mid].Get(input_str_[pos]) = leaf;

        ptr_.first = GetLink(mid);
        ptr_.second = nodes_[ptr_.first].Len();
        if (!mid) {
            fl = false;
            break;
        }
    }
}

std::pair<int_fast32_t, int_fast32_t> Trie::Go(std::pair<int_fast32_t, int_fast32_t> st,
                                               int_fast32_t l, int_fast32_t r) {
    while (l < r) {
        if (st.second == nodes_[st.first].Len()) {
            st = std::make_pair(nodes_[st.first].Get(input_str_.at(l)), 0);
            if (st.first == -1) {
                return st;
            }
        } else {
            if (input_str_[nodes_[st.first].l + st.second] != input_str_.at(l)) {
                return std::make_pair(-1, -1);
            }
            if (r - l < nodes_[st.first].Len() - st.second) {
                return std::make_pair(st.first, st.second + r - l);
            }
            l += nodes_[st.first].Len() - st.second;
            st.second = nodes_[st.first].Len();
        }
    }
    return st;
}

std::pair<int_fast32_t, int_fast32_t> Trie::GoOnes(std::pair<int_fast32_t, int_fast32_t> st,
                                                   int_fast32_t l, const char *s) {
    int_fast32_t r = l + 1;
    bool fl = true;
    while (fl) {
        if (st.second == nodes_[st.first].Len()) {
            st = std::make_pair(nodes_[st.first].Get(*s), 0);
            if (st.first == -1) {
                fl = false;
                return st;
            }
        } else {
            if (input_str_[nodes_[st.first].l + st.second] != *s) {
                fl = false;
                return std::make_pair(-1, -1);
            }
            if (r - l < nodes_[st.first].Len() - st.second) {
                fl = false;
                return std::make_pair(st.first, st.second + r - l);
            }
            st.second = nodes_[st.first].Len();
            fl = false;
            return st;
        }
    }
    return st;
}

bool Trie::IsContains(std::string *stri) {
    if (!nodes_[0].children.count(stri->at(0))) {
        return false;
    }
    if (stri->size() > input_str_.size()) {
        return false;
    }

    std::pair<int32_t, int32_t> st = std::make_pair(0, 0);

    for (int_fast32_t i = 0; i < static_cast<int_fast32_t>(stri->size()); ++i) {
        st = GoOnes(st, i, &stri->at(i));

        if (st.first == -1) {
            return false;
        }
    }
    return true;
}

void Trie::GetAns(std::string *operation, std::string *stri) {
    if (*operation == "?") {
        if (IsContains(stri)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }

    if (*operation == "A") {
        for (char i : *stri) {
            AddSymbol(i);
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string operation;
    std::string input_str;

    Trie trie = Trie();

    while (std::cin >> operation >> input_str) {
        std::transform(input_str.begin(), input_str.end(), input_str.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        trie.GetAns(&operation, &input_str);
    }
    return 0;
}
*/
