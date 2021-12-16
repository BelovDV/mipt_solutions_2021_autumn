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
#define LOG_IN                                                          \
    for (int i = 0; i < log_depth; ++i) {                               \
        std::cout << "      ";                                          \
    }                                                                   \
    std::cout << "\033[1m\033[4m" << __PRETTY_FUNCTION__ << "\033[m\n"; \
    ++log_depth;
#define LOG_OUT --log_depth;
#define LOG(var)                          \
    for (int i = 0; i < log_depth; ++i) { \
        std::cout << "      ";            \
    }                                     \
    std::cout << "LOG: " << __FILE__ << ':' << __LINE__ << " \t" << #var << ": \t" << var << "\n";
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

class SuffixAutomaton {
public:
    using Index = unsigned;
    using Symbol = char;
    using String = std::string;
    static const Index kNil = static_cast<Index>(-1);
    static const Index kRoot = 0;

private:
    class Node {
    public:
        static const int kAlphaSize = 26;

    public:
        explicit Node(Index length = 0) : length_(length), suffix_(kNil) {
            for (auto& it : next_) {
                it = kNil;
            }
        }

    public:
        bool Linked(Symbol direction) {
            return next_[direction - 'a'] != kNil;
        }
        Index& Link(Symbol direction) {
            return next_[direction - 'a'];
        }

    public:
        bool endpos = false;
        Index length_;
        Index suffix_;
        std::array<Index, kAlphaSize> next_;
    };

public:
    explicit SuffixAutomaton(const String& base) {
        data_[0].suffix_ = kNil;
        for (const auto& it : base) {
            Insert(it);
        }
    }

public:
    void InsertAdd(String::const_iterator begin, String::const_iterator end) {
        for (auto iter = begin; iter != end; ++iter) {
            Insert(*iter);
        }
    }

    size_t MaxOverlapLength(const String& str) {
        size_t length = 0;
        Index state = 0;
        size_t depth = 0;
        for (const auto& dir : str) {
            depth += 1;
            if (data_[state].Linked(dir)) {
                state = data_[state].Link(dir);
                if (data_[state].endpos) {
                    length = depth;
                }
            } else {
                break;
            }
        }
        return length;
    }

    void Dump(const std::string& filename) {
        auto file = fopen(filename.c_str(), "w");
        fprintf(file, "digraph G {\nrankdir=LR;\n");
        for (Index i = 0; i < data_.size(); ++i) {
            if (data_[i].endpos) {
                fprintf(file, " %u [shape=doublecircle]\n", i);
            } else {
                fprintf(file, " %u [shape=circle]\n", i);
            }
            for (Symbol dir = 'a'; dir <= 'z'; ++dir) {
                if (data_[i].Linked(dir)) {
                    fprintf(file, "\t%u->%u[label=%c]\n", i, data_[i].Link(dir), dir);
                }
            }
            fprintf(file, "%u->%u[color=lightgray]\n", i, data_[i].suffix_);
        }
        fprintf(file, "}");
        fclose(file);
    }

private:
    void Insert(Symbol next) {
        LOG_IN
        LOG(next)
        Index current = data_.size();
        data_.emplace_back(data_[last_].length_ + 1);
        for (auto iter = last_; iter != kNil; iter = data_[iter].suffix_) {
            data_[iter].endpos = false;
        }
        Index iter = last_;
        LOG(iter)
        while (iter != kNil && !data_[iter].Linked(next)) {
            data_[iter].Link(next) = current;
            iter = data_[iter].suffix_;
            LOG(iter)
        }
        if (iter == kNil) {
            LOG(1)
            data_[current].suffix_ = kRoot;
        } else {
            auto was_to = data_[iter].Link(next);
            LOG(was_to)
            LOG(data_[iter].length_)
            LOG(data_[was_to].length_)
            if (data_[iter].length_ + 1 == data_[was_to].length_) {
                LOG(2)
                data_[current].suffix_ = was_to;
            } else {
                LOG(3)
                Index clone = data_.size();
                data_.emplace_back(data_[iter].length_ + 1);
                data_[clone].next_ = data_[was_to].next_;
                data_[clone].suffix_ = data_[was_to].suffix_;
                while (iter != kNil && data_[iter].Link(next) == was_to) {
                    data_[iter].Link(next) = clone;
                    iter = data_[iter].suffix_;
                }
                data_[was_to].suffix_ = data_[current].suffix_ = clone;
            }
        }
        last_ = current;
        for (iter = current; iter != kNil; iter = data_[iter].suffix_) {
            data_[iter].endpos = true;
        }
        LOG_OUT
    }

private:
    std::vector<Node> data_ = std::vector<Node>(1);
    Index last_ = 0;
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

using std::cin;
using std::cout;
using std::vector;

void WorkTrie() {
    size_t input_size = 0;
    cin >> input_size;
    std::string input;
    SuffixAutomaton state("");
    std::string result;
    for (size_t i = 0; i < input_size; ++i) {
        cin >> input;
        auto overlap = state.MaxOverlapLength(input);
        auto begin = input.begin() + overlap;
        auto end = input.end();
        result += std::string(begin, end);
        state.InsertAdd(begin, end);
    }
    cout << result << '\n';
#ifdef MY
    state.Dump("graph");
#endif
}

int main() {
#ifndef MY
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
    WorkTrie();
}
