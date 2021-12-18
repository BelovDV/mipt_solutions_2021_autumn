#pragma GCC optimize("O2")

#include <vector>
#include <iostream>
#include <memory>
#include <array>
#include <map>
#include <cassert>

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

#undef MY

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
        explicit Node(size_t length = 0) : suffix_(kNil), length_(length) {
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
        void SetSuffix(Index suffix) {
            suffix_ = suffix;
        }
        Index Suffix() {
            return suffix_;
        }

    private:
        Index suffix_ = 0;

    public:
        std::array<Index, kAlphaSize> next_;
        bool endpos = false;
        size_t length_ = 0;
    };

public:
    explicit SuffixAutomaton(const String& base) {
        data_[0].SetSuffix(kNil);
        for (const auto& it : base) {
            Insert(it);
        }
        data_.reserve(200005);
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

#ifdef MY
    void Dump(const std::string& filename) {
        auto file = fopen(filename.c_str(), "w");
        fprintf(file, "digraph G {\nrankdir=LR;\n");
        for (Index i = 0; i < data_.size(); ++i) {
            if (data_[i].endpos) {
                fprintf(file, " %u [shape=doublecircle label=_%u_%lu]\n", i, i, data_[i].length_);
            } else {
                fprintf(file, " %u [shape=circle label=_%u_%lu]\n", i, i, data_[i].length_);
            }
            for (Symbol dir = 'a'; dir <= 'z'; ++dir) {
                if (data_[i].Linked(dir)) {
                    fprintf(file, "\t%u->%u[label=%c]\n", i, data_[i].Link(dir), dir);
                }
            }
            fprintf(file, "%u->%u[color=lightgray]\n", i, data_[i].Suffix());
        }
        fprintf(file, "}");
        fclose(file);
    }
#endif

    size_t Insert(Symbol next) {
        LOG_IN
        LOG(next)
        Index current = data_.size();
        data_.emplace_back(data_[last_].length_ + 1);
#if 0  // THIS MAKE IT SEVERAL TIME FASTER
        for (auto iter = last_; iter != kNil; iter = data_[iter].Suffix()) {
            data_[iter].endpos = false;
        }
#endif
        Index iter = last_;
        LOG(iter)
        while (iter != kNil && !data_[iter].Linked(next)) {
            data_[iter].Link(next) = current;
            iter = data_[iter].Suffix();
            LOG(iter)
        }
        if (iter == kNil) {
            LOG(1)
            data_[current].SetSuffix(kRoot);
            count_substring_ += data_[current].length_;
        } else {
            auto was_to = data_[iter].Link(next);
            LOG(was_to)
            LOG(data_[iter].length_)
            LOG(data_[was_to].length_)
            if (data_[iter].length_ + 1 == data_[was_to].length_) {
                LOG(2)
                data_[current].SetSuffix(was_to);
                count_substring_ += data_[current].length_ - data_[was_to].length_;
            } else {
                LOG(3)
                Index clone = data_.size();
                data_.emplace_back(data_[iter].length_ + 1);
                data_[clone].next_ = data_[was_to].next_;
                data_[clone].SetSuffix(data_[was_to].Suffix());
                LOG(count_substring_)
                count_substring_ += data_[clone].length_ - data_[data_[was_to].Suffix()].length_;
                LOG(count_substring_)
                LOG("inc")
                while (iter != kNil && data_[iter].Link(next) == was_to) {
                    data_[iter].Link(next) = clone;
                    iter = data_[iter].Suffix();
                }
                ChangeSuffix(was_to, clone);
                data_[current].SetSuffix(clone);
                count_substring_ += data_[current].length_ - data_[clone].length_;
            }
        }
        last_ = current;
#if 0  // THIS MAKE IT SEVERAL TIME FASTER
        for (iter = current; iter != kNil; iter = data_[iter].Suffix()) {
            data_[iter].endpos = true;
        }
#endif
        LOG_OUT
        return count_substring_;
    }

private:
    void ChangeSuffix(Index vertex, Index new_suffix) {
        LOG_IN
        LOG(vertex)
        LOG(data_[vertex].Suffix())
        LOG(new_suffix)
        LOG(count_substring_)
        count_substring_ -= data_[vertex].length_ - data_[data_[vertex].Suffix()].length_;
        LOG(count_substring_)
        data_[vertex].SetSuffix(new_suffix);
        count_substring_ += data_[vertex].length_ - data_[new_suffix].length_;
        LOG(count_substring_)
        LOG_OUT
    }

private:
    std::vector<Node> data_ = std::vector<Node>(1);
    Index last_ = 0;
    size_t count_substring_ = 0;
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

using std::cin;
using std::cout;
using std::vector;

#include <set>

void WorkTrie() {
    std::string input;
    input.reserve(100005);
    cin >> input;
    auto automaton = SuffixAutomaton("");
    // size_t old = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        auto result = automaton.Insert(input[i]);
#if 0
        if (result < old) {
            cout << "FIND\n";
            break;
        }
        old = result;
#endif
        cout << result << '\n';
#ifdef MY
        std::set<std::string> vsp;
        for (size_t f = 0; f < i + 1; ++f) {
            for (size_t t = f + 1; t <= i + 1; ++t) {
                vsp.insert(std::string(input.begin() + f, input.begin() + t));
            }
        }
        if (vsp.size() != result) {
            LOG(result)
            LOG(vsp.size())
            LOG(std::string(input.begin(), input.begin() + i))
            LOG("AAAAAAAAAAAAAAAAA")
            break;
        }
#endif
    }

#ifdef MY
    automaton.Dump("graph");
#endif
}

void Test() {
    std::string input(150, 'a');
    while (true) {
        cout << input << '\n';
        auto suf = SuffixAutomaton("");
        for (size_t i = 0; i < input.size() - 1; ++i) {
            suf.Insert(input[i]);
        }
        auto res = suf.Insert(input.back());
        std::set<std::string> vsp;
        for (size_t f = 0; f < input.size(); ++f) {
            for (size_t t = f + 1; t <= input.size(); ++t) {
                vsp.insert(std::string(input.begin() + f, input.begin() + t));
            }
        }
        if (vsp.size() != res) {
            cout << res << '\n';
            cout << vsp.size() << '\n';
            cout << input << '\n';
            cout << "AAAAAAAAAAAAAAAAA" << '\n';
            break;
        }
        for (size_t i = 0; i < input.size(); ++i) {
            static unsigned vsp = 12;
            vsp += input[i] * input[i];
            input[i] = (vsp % 5) + 'a';
        }
#if 0
        auto i = input.size() - 1;
        do {
            if (input[i] != 'f') {
                if (i == 2) {
                    cout << input << '\n';
                }
                input[i]++;
                for (auto j = i + 1; j < input.size(); ++j) {
                    input[j] = 'a';
                }
                break;
            }
        } while (i-- != 0);
        if (input == "zzz") {
            break;
        }
#endif
    }
}
#include <unordered_set>
void Test1() {
    std::string input;
    cin >> input;
    auto automaton = SuffixAutomaton("");
    std::unordered_set<std::string> vsp;
    cout << input << '\n';
    for (size_t i = 0; i < input.size(); ++i) {
        auto result = automaton.Insert(input[i]);
        cout << result << '\n';
        for (size_t f = 0; f < i + 1; ++f) {
            vsp.insert(std::string(input.begin() + f, input.begin() + i + 1));
        }
        if (vsp.size() != result) {
            cout << ("AAAAAAAAAAAAAAAAA") << '\n';
            cout << (result) << '\n';
            cout << (vsp.size()) << '\n';
            cout << (std::string(input.begin(), input.begin() + i)) << '\n';
            break;
        }
    }
}

int main() {
#ifndef MY
    cin.tie(nullptr)->sync_with_stdio(false);
    cout.tie(nullptr)->sync_with_stdio(false);
#endif
    WorkTrie();
    // Test1();
}
