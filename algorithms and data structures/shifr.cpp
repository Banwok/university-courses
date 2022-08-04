/*
#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <memory>
#include <vector>
#include <fstream>

class CartesianTree {
    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        int value = 0;
        char key = 0;
        int subTreeSize = 0;
    };

protected:
    std::shared_ptr<Node> tree_root_ = nullptr;
    std::mt19937 generator_;
    std::uniform_int_distribution<int> distribution_;

    struct Dpair {
        std::shared_ptr<Node> first = nullptr;
        std::shared_ptr<Node> second = nullptr;
    };

public:
    explicit CartesianTree() = default;

    ~CartesianTree() = default;

    int LeftSubTreeSize(std::shared_ptr<Node> &node) {
        if (node == nullptr) {
            return 0;
        }
        if (node->left != nullptr) {
            return node->left->subTreeSize;
        }
        return 0;
    }

    int RightSubTreeSize(std::shared_ptr<Node> &node) {
        if (node == nullptr) {
            return 0;
        }
        if (node->right != nullptr) {
            return node->right->subTreeSize;
        }
        return 0;
    }

    Dpair SplitS(std::shared_ptr<Node> &root, int pos) {
        Dpair answer;
        int left_size;
        if (root == nullptr) {
            return answer;
        } else if (root->left == nullptr) {
            left_size = 0;
        } else {
            left_size = root->left->subTreeSize;
        }

        if (left_size < pos) {
            answer = SplitS(root->right, pos - left_size - 1);
            root->right = answer.first;
            answer.first = root;
        } else {
            answer = SplitS(root->left, pos);
            root->left = answer.second;
            answer.second = root;
        }
        if (root != nullptr) {
            root->subTreeSize = LeftSubTreeSize(root) + RightSubTreeSize(root) + 1;
        }
        return answer;
    }

    std::shared_ptr<Node> Merge(std::shared_ptr<Node> first, std::shared_ptr<Node> second) {
        if (first == nullptr) {
            return second;
        } else if (second == nullptr) {
            return first;
        } else if (first->value > second->value) {
            first->right = Merge(first->right, second);
            if (first != nullptr) {
                first->subTreeSize = LeftSubTreeSize(first) + RightSubTreeSize(first) + 1;
            }
            return first;
        }
        second->left = Merge(first, second->left);
        if (second != nullptr) {
            second->subTreeSize = LeftSubTreeSize(second) + RightSubTreeSize(second) + 1;
        }
        return second;
    }

    void Insert(char key) {
        std::shared_ptr<Node> new_node(new Node);
        new_node->value = distribution_(generator_);
        new_node->key = key;
        new_node->subTreeSize = 1;
        tree_root_ = Merge(tree_root_, new_node);
    }

    char Delete() {
        auto splt_f = SplitS(tree_root_, 1);
        tree_root_ = splt_f.second;
        return splt_f.first->key;
    }

    void Request(int first, int last, int shift) {
        if (shift != 0) {
            auto splt_f = SplitS(tree_root_, last);
            auto splt_s = SplitS(splt_f.first, first);
            auto splt_t = SplitS(splt_s.second, shift);
            splt_s.second = Merge(splt_t.second, splt_t.first);
            splt_f.first = Merge(splt_s.first, splt_s.second);
            tree_root_ = Merge(splt_f.first, splt_f.second);
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    CartesianTree tree;

    std::ifstream fin;
    fin.open("input.txt");

    std::string sequence;
    fin >> sequence;

    size_t size = sequence.size();
    for (size_t i = 0; i < size; ++i) {
        tree.Insert(sequence[i]);
    }
    int n_value;
    fin >> n_value;
    std::vector<int> start(n_value);
    std::vector<int> finish(n_value);
    std::vector<int> shift(n_value);
    for (int ind = 0; ind < n_value; ++ind) {
        fin >> start[ind] >> finish[ind] >> shift[ind];
    }
    for (int ind = n_value - 1; ind >= 0; --ind) {
        tree.Request(start[ind] - 1, finish[ind], shift[ind]);
    }
    for (size_t i = 0; i < size; ++i) {
        sequence[i] = tree.Delete();
    }

    std::ofstream fout;
    fout.open("output.txt");
    fout << sequence;

    return 0;
}
*/
