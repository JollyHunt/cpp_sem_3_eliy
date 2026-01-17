#ifndef FREQUENT_SUBSTRINGS_H
#define FREQUENT_SUBSTRINGS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Trie.h"
#include "table/CustomHashTable.h"
#include "sequence/ArraySequence.h"

inline ArraySequence<char> stringToArray(const std::string& s) {
    ArraySequence<char> seq(s.length());
    for (size_t i = 0; i < s.length(); ++i) {
        seq.Set(i, s[i]);
    }
    return seq;
}

inline std::string arrayToString(const ArraySequence<char>& arr) {
    std::string s;
    for (size_t i = 0; i < arr.Size(); ++i) {
        s += arr.Get(i);
    }
    return s;
}

std::vector<std::pair<std::string, int>> findFrequentSubstringsCustom(
    const std::string& s,
    int lmin,
    int lmax
) {
    if (lmin < 1 || lmax < lmin || lmax > static_cast<int>(s.length())) {
        throw std::invalid_argument("Invalid lmin/lmax");
    }

    CustomHashTable<ArraySequence<char>, int> freqMap;

    for (size_t i = 0; i <= s.length(); ++i) {
        for (int len = lmin; len <= lmax; ++len) {
            if (i + len <= s.length()) {
                std::string substr = s.substr(i, len);
                ArraySequence<char> key = stringToArray(substr);

                if (freqMap.ContainsKey(key)) {
                    int count = freqMap.Get(key);
                    freqMap.Add(key, count + 1);
                } else {
                    freqMap.Add(key, 1);
                }
            }
        }
    }

    auto keys = freqMap.GetKeys();
    std::vector<std::pair<std::string, int>> result;
    for (size_t i = 0; i < keys.Size(); ++i) {
        result.push_back({arrayToString(keys.Get(i)), freqMap.Get(keys.Get(i))});
    }

    std::sort(result.begin(), result.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    return result;
}

std::vector<std::pair<std::string, int>> findFrequentSubstringsSTL(
    const std::string& s,
    int lmin,
    int lmax
) {
    if (lmin < 1 || lmax < lmin || lmax > static_cast<int>(s.length())) {
        throw std::invalid_argument("Invalid lmin/lmax");
    }

    std::unordered_map<std::string, int> freqMap;

    for (size_t i = 0; i <= s.length(); ++i) {
        for (int len = lmin; len <= lmax; ++len) {
            if (i + len <= s.length()) {
                std::string substr = s.substr(i, len);
                freqMap[substr]++;
            }
        }
    }

    std::vector<std::pair<std::string, int>> result(freqMap.begin(), freqMap.end());
    std::sort(result.begin(), result.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    return result;
}

#endif