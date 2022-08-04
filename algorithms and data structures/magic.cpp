/*
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>

struct Pair {
    int first = 0;
    int second = 0;

    bool operator<(const Pair &pair) const {
        if (this->first == pair.first) {
            return this->second < pair.second;
        }
        return this->first < pair.first;
    }
};

class Graph {
public:
    struct Node {
        int component = -1;
        std::vector<int> children;

        void AddEdge(int child) {
            children.push_back(child);
        }
    };

    std::vector<Node> graph_;
    std::vector<Node> cgraph_;
    size_t size_ = 0;
    int timer_ = 0;
    std::vector<bool> used_;
    std::vector<int> time_in_;
    std::vector<int> fup_;
    std::vector<int> lca_h_;
    std::vector<int> lca_dfs_list_;
    std::vector<int> lca_first_;
    std::vector<int> lca_tree_;

    int maxcolor_ = -1;
    int capital_ = -1;

    void Dfs(int v_index, int p_value = -1) {
        used_[v_index] = true;
        ++timer_;
        time_in_[v_index] = timer_;
        fup_[v_index] = timer_;
        size_t cur_size = graph_[v_index].children.size();
        for (size_t ind = 0; ind < cur_size; ++ind) {
            int next = graph_[v_index].children[ind];
            if (next != p_value) {
                if (!used_[next]) {
                    Dfs(next, v_index);
                    fup_[v_index] = std::min(fup_[v_index], fup_[next]);
                } else {
                    fup_[v_index] = std::min(fup_[v_index], fup_[next]);
                }
            } else {
                continue;
            }
        }
    }

    void Paint(int v_index, int color) {
        graph_[v_index].component = color;
        size_t size = graph_[v_index].children.size();
        for (size_t i = 0; i < size; ++i) {
            int child = graph_[v_index].children[i];
            if (graph_[child].component == -1) {
                if (fup_[child] <= time_in_[v_index]) {
                    Paint(child, color);
                } else {  //----------
                    ++maxcolor_;
                    Paint(child, maxcolor_);
                }
            }
        }
    }

    void LcaDfs(int v_index, int height = 1) {
        used_[v_index] = true;
        lca_h_[v_index] = height;
        lca_dfs_list_.push_back(v_index);
        size_t size = cgraph_[v_index].children.size();
        for (size_t i = 0; i < size; ++i) {
            int child = cgraph_[v_index].children[i];
            if (!used_[child]) {
                LcaDfs(child, height + 1);
                lca_dfs_list_.push_back(v_index);
            }
        }
    }

    void LcaBuildTree(int i_value, int l_value, int r_value) {
        if (l_value != r_value) {
            int m_value = (l_value + r_value) / 2;
            LcaBuildTree(i_value + i_value, l_value, m_value);
            LcaBuildTree(i_value + i_value + 1, m_value + 1, r_value);
            if (lca_h_[lca_tree_[i_value + i_value]] < lca_h_[lca_tree_[i_value + i_value + 1]]) {
                lca_tree_[i_value] = lca_tree_[i_value + i_value];
            } else {
                lca_tree_[i_value] = lca_tree_[i_value + i_value + 1];
            }
        } else {
            lca_tree_[i_value] = lca_dfs_list_[l_value];
        }
    }

    int LcaTreeMin(int ii, int sl, int sr, int ll, int rr) {
        if ((sl == ll) && (sr == rr)) {
            return lca_tree_[ii];
        } else {
            int sm = (sl + sr) / 2;
            if (rr <= sm) {
                return LcaTreeMin(ii + ii, sl, sm, ll, rr);
            } else if (ll > sm) {
                return LcaTreeMin(ii + ii + 1, sm + 1, sr, ll, rr);
            } else {
                int ans_f = LcaTreeMin(ii + ii, sl, sm, ll, sm);
                int ans_s = LcaTreeMin(ii + ii + 1, sm + 1, sr, sm + 1, rr);
                return lca_h_[ans_f] < lca_h_[ans_s] ? ans_f : ans_s;
            }
        }
    }

    int Lca(int aa, int bb) {
        int left = lca_first_[aa];
        int right = lca_first_[bb];
        if (left > right) {
            std::swap(left, right);
        }
        return LcaTreeMin(1, 0, static_cast<int>(lca_dfs_list_.size() - 1), left, right);
    }

    explicit Graph() = default;

    ~Graph() = default;

    void Solve() {
        int edges;
        std::ifstream fin;
        fin.open("input.txt");
        fin >> size_ >> edges >> capital_;
        int first, second;
        graph_.resize(size_);
        for (int i = 0; i < edges; ++i) {
            fin >> first >> second;
            --first;
            --second;
            graph_[first].AddEdge(second);
            graph_[second].AddEdge(first);
        }
        used_.resize(size_, false);
        time_in_.resize(size_);
        fup_.resize(size_);
        timer_ = 0;
        for (size_t i = 0; i < size_; ++i) {
            if (!used_[i]) {
                Dfs(i);
            }
        }

        maxcolor_ = -1;
        for (size_t i = 0; i < size_; ++i) {
            if (graph_[i].component == -1) {
                ++maxcolor_;
                Paint(i, maxcolor_);
            }
        }
        used_.assign(size_, false);

        std::set<Pair> edges_set;
        edges_set.clear();
        cgraph_.resize(static_cast<unsigned int>(maxcolor_ + 1));
        for (size_t ind = 0; ind < size_; ++ind) {
            size_t children_size = graph_[ind].children.size();
            for (size_t j = 0; j < children_size; ++j) {
                auto child = graph_[ind].children[j];
                if (graph_[child].component != graph_[ind].component) {
                    Pair edge;
                    edge.first = graph_[child].component;
                    edge.second = graph_[ind].component;
                    if (edges_set.find(edge) == edges_set.end()) {
                        edges_set.insert(edge);
                    }
                }
            }
        }
        for (auto edg_it = edges_set.begin(); edg_it != edges_set.end(); ++edg_it) {
            cgraph_[edg_it->first].AddEdge(edg_it->second);
        }

        size_t nn = cgraph_.size();
        lca_h_.resize(cgraph_.size());
        lca_dfs_list_.reserve(2 * cgraph_.size());
        LcaDfs(graph_[capital_ - 1].component, 0);
        size_t mm = lca_dfs_list_.size();

        lca_tree_.assign(lca_dfs_list_.size() * 4 + 1, -1);
        LcaBuildTree(1, 0, mm - 1);
        lca_first_.assign(nn, -1);
        for (size_t i = 0; i < mm; ++i) {
            int vv = lca_dfs_list_[i];
            if (lca_first_[vv] == -1) {
                lca_first_[vv] = i;
            }
        }

        std::ofstream fout;
        fout.open("output.txt");
        int req = 0, fr, sc;
        fin >> req;
        for (int i = 0; i < req; ++i) {
            fin >> fr >> sc;
            int ans = Lca(graph_[fr - 1].component, graph_[sc - 1].component);
            fout << lca_h_[ans];
            if (i != req - 1) {
                fout << '\n';
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Graph solver;
    solver.Solve();
    return 0;
}
*/
