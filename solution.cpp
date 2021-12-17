#include <vector>
#include <iostream>
#include <memory>
#include <array>
#include <map>
#include <cassert>

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

#ifndef MY
#define LOG(var)
#define LOG_IN
#define LOG_OUT
#else
#define DEBUG
int log_depth = 0;
#define LOG_IN                                \
    for (int i = 0; i < log_depth; ++i) {     \
        std::cout << "    ";                  \
    }                                         \
    std::cout << __PRETTY_FUNCTION__ << '\n'; \
    ++log_depth;
#define LOG_OUT --log_depth;
#define LOG(var)                                                \
    for (int i = 0; i < log_depth; ++i) {                       \
        std::cout << "    ";                                    \
    }                                                           \
    std::cout << "LOG: " << __FILE__ << ':' << __LINE__ << " "; \
    printf("%20.20s", #var);                                    \
    std::cout << ": \t" << var << "\n";
#endif

template <typename Cont, typename = typename std::enable_if_t<!std::is_same<Cont, std::string>::value>>
auto operator<<(std::ostream& out, const Cont& cont) -> decltype(cont.begin(), out) {
    for (auto it : cont) {
        out << it << '\t';
    }
    out << '\n';
    return out;
}
template <typename Cont>
auto operator<<(std::ostream& out, const Cont& cont) -> decltype(cont.first, cont.second, out) {
    out << cont.first << ' ' << cont.second;
    return out;
}

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //
class Node {
public:
    using Symbol = char;
    static const size_t kNil = static_cast<size_t>(-1);

public:
    Node() {
        for (auto& it : next_) {
            it = kNil;
        }
    }

public:
    Node Expand(Symbol dir, size_t dest) {
        next_[dir - 'a'] = dest;
        return Node();
    }
    size_t Next(Symbol dir) {
        return next_[dir - 'a'];
    }
    void SetSuffix(size_t dest) {
        suffix_ = dest;
    }
    size_t Suffix() {
        return suffix_;
    }
    const auto& AllNext() {
        return next_;
    }

private:
    // std::map<Symbol, size_t> next_;
    std::array<size_t, 26> next_;
    size_t suffix_ = 0;

    // ===== //
public:
    std::vector<size_t> ends_;
};

class Trie {
public:
    using Symbol = Node::Symbol;

public:
    Trie() {
        Current().SetSuffix(Node::kNil);
    }

public:
    template <typename Iter>
    bool Insert(Iter begin, Iter end) {
        bool was_inserted = false;
        state_ = 0;
        for (auto iter = begin; iter != end; ++iter) {
            if (!StateMove(*iter)) {
                was_inserted = true;
                data_.push_back(Current().Expand(*iter, data_.size()));
                StateMove(*iter);
            }
        }
        return was_inserted;
    }

    void RecalculateSuffixes() {
        std::vector<size_t> current(1, 0);
        std::vector<size_t> storage;
        while (!current.empty()) {
            for (auto cur : current) {
                for (Symbol dir = 'a'; dir <= 'z'; ++dir) {
                    auto updated = data_[cur].AllNext()[dir - 'a'];
                    if (updated == Node::kNil) {
                        continue;
                    }
                    storage.push_back(updated);
                    auto iter = data_[cur].Suffix();
                    while (iter != Node::kNil && data_[iter].Next(dir) == Node::kNil) {
                        iter = data_[iter].Suffix();
                    }
                    if (iter == Node::kNil) {
                        data_[updated].SetSuffix(0);
                    } else {
                        data_[updated].SetSuffix(data_[iter].Next(dir));
                    }
                }
            }
            std::swap(storage, current);
            storage.clear();
        }
    }

    Node& Current() {
        return data_[state_];
    }
    void StateRoot() {
        state_ = 0;
    }
    bool StateMove(size_t& state, Symbol dir) {
        if (data_[state].Next(dir) == Node::kNil) {
            return false;
        }
        state = data_[state].Next(dir);
        return true;
    }
    bool StateMove(Symbol dir) {
        return StateMove(state_, dir);
    }
    bool StateMoveSuffix(size_t& state) {
        if (data_[state].Suffix() == Node::kNil) {
            return false;
        }
        state = data_[state].Suffix();
        return true;
    }
    bool StateMoveSuffix() {
        return StateMoveSuffix(state_);
    }
    size_t StateGet() {
        return state_;
    }
    bool StateSet(size_t state) {
        if (state >= data_.size()) {
            return false;
        }
        state_ = state;
        return true;
    }
    Node& Get(size_t state) {
        return data_[state];
    }
    void Dump(const std::string& filename) {
        auto file = fopen(filename.c_str(), "w");
        fprintf(file, "digraph G {\nrankdir=LR;\n");
        for (size_t i = 0; i < data_.size(); ++i) {
            if (!data_[i].ends_.empty()) {
                fprintf(file, " %lu [shape=doublecircle]\n", i);
            } else {
                fprintf(file, " %lu [shape=circle]\n", i);
            }
            for (Symbol dir = 'a'; dir <= 'z'; ++dir) {
                if (data_[i].Next(dir) != Node::kNil) {
                    fprintf(file, "\t%lu->%lu[label=%c]\n", i, data_[i].Next(dir), dir);
                }
            }
            fprintf(file, "%lu->%lu[color=lightgray]\n", i, data_[i].Suffix());
        }
        fprintf(file, "}");
        fclose(file);
    }

private:
    size_t state_ = 0;
    std::vector<Node> data_ = std::vector<Node>(1);
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

using std::cin;
using std::cout;
using std::vector;

void WorkTrie() {
    std::string text;
    cin >> text;
    size_t input_size = 0;
    cin >> input_size;
    std::vector<std::string> input(input_size);
    auto trie = Trie();
    for (size_t i = 0; i < input_size; ++i) {
        cin >> input[i];
        trie.Insert(input[i].begin(), input[i].end());
        trie.Current().ends_.push_back(i);
    }
    trie.RecalculateSuffixes();

#ifdef MY
    trie.Dump("graph");
#endif

    std::vector<std::vector<size_t>> occurs(input_size);
    size_t state = 0;
    for (size_t position = 0; position < text.size(); ++position) {
        while (!trie.StateMove(state, text[position])) {
            if (!trie.StateMoveSuffix(state)) {
                state = 0;
                break;
            }
        }
        LOG(state)
        auto iter = state;
        do {
            for (auto number : trie.Get(iter).ends_) {
                occurs[number].emplace_back(position + 1 - input[number].size());
            }
        } while (trie.StateMoveSuffix(iter));
    }
    for (const auto& pattern : occurs) {
        cout << pattern.size() << ' ';
        for (auto it : pattern) {
            cout << it + 1 << ' ';
        }
        cout << '\n';
    }
}

int main() {
#ifndef MY
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
    WorkTrie();
}
