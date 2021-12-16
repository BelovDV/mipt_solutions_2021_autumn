#include <vector>
#include <iostream>
#include <memory>
#include <array>
#include <map>

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
#define LOG(var)                                   \
    for (int i = 0; i < log_depth; ++i) {          \
        std::cout << "    ";                       \
    }                                              \
    printf("LOG: %d '%20.20s': ", __LINE__, #var); \
    std::cout << var << "\n";
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
    Node() = default;

public:
    Node Expand(Symbol dir, size_t dest) {
        next_[dir] = dest;
        return Node();
    }
    size_t Next(Symbol dir) {
        return next_.count(dir) ? next_[dir] : kNil;
    }

private:
    std::map<Symbol, size_t> next_;

    // ===== //
public:
    size_t count_ = 0;
    size_t cost_ = 0;
    bool leaf_ = false;
};

class Trie {
public:
    using Symbol = Node::Symbol;

public:
    Trie() = default;

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

    Node& Current() {
        return data_[state_];
    }
    void StateRoot() {
        state_ = 0;
    }
    bool StateMove(Symbol dir) {
        if (Current().Next(dir) == Node::kNil) {
            return false;
        }
        state_ = data_[state_].Next(dir);
        return true;
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

private:
    size_t state_ = 0;
    std::vector<Node> data_ = std::vector<Node>(1);
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

using std::cin;
using std::cout;
using std::vector;

#include <unordered_map>
#include <set>

std::string position;

Trie trie;
int foot;
std::vector<size_t> payment;
std::string best_string;
size_t best_cost = UINT64_MAX;
size_t length = 0;

void CountCosts(size_t upper_payment) {
    LOG_IN
    LOG(position)
    auto state = trie.StateGet();
    auto count = trie.Current().count_;
    for (char dir = '0'; dir < '0' + foot; ++dir) {
        if (trie.StateMove(dir)) {
            position.push_back(dir);
            CountCosts(upper_payment + (count - trie.Current().count_) * payment[position.size() - 2]);
            position.pop_back();
            trie.StateSet(state);
        } else {
            auto cost = upper_payment + trie.Current().count_ * payment[position.size() - 1];
            LOG(cost)
            if (cost < best_cost) {
                best_cost = cost;
                best_string = position;
                while (best_string.size() < length) {
                    best_string.push_back(dir);
                }
            }
            break;
        }
    }
    LOG_OUT
}

void WorkTrie() {
    size_t number_string = 0;
    cin >> number_string >> length >> foot;
    payment.resize(length);
    for (auto& it : payment) {
        cin >> it;
    }
    while (number_string--) {
        std::string input;
        cin >> input;
        trie.Insert(input.begin(), input.end());
        trie.StateRoot();
        for (auto dir : input) {
            trie.StateMove(dir);
            trie.Current().count_ += 1;
        }
    }
    for (char dir = '0'; dir < '0' + foot; ++dir) {
        trie.StateRoot();
        if (trie.StateMove(dir)) {
            position.push_back(dir);
            CountCosts(0);
            position.pop_back();
        } else {
            while (length--) {
                position.push_back(dir);
            }
            cout << position << '\n' << 0 << '\n';
            return;
        }
    }
    cout << best_string << '\n' << best_cost << '\n';
}

int main() {
#ifndef MY
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
    WorkTrie();
}
