#ifndef TRIE_H
#define TRIE_H

#include "sequence/ArraySequence.h"
#include "sequence/ListSequence.h"

template <typename K, typename V>
struct Pair {
    K first;
    V second;

    Pair() = default;
    Pair(const K& k, const V& v) : first(k), second(v) {}
};

template <typename T>
class TrieNode {
public:
    ArraySequence<Pair<T, TrieNode<T>*>> children;
    int count = 0;
    bool isEnd = false;

    ~TrieNode() {
        for (size_t i = 0; i < children.Size(); ++i) {
            delete children.Get(i).second;
        }
    }

    TrieNode<T>* findChild(const T& ch) {
        for (size_t i = 0; i < children.Size(); ++i) {
            if (children.Get(i).first == ch) {
                return children.Get(i).second;
            }
        }
        return nullptr;
    }

    void addChild(const T& ch, TrieNode<T>* node) {
        children.Append(Pair<T, TrieNode<T>*>(ch, node));
    }
};

template <typename T>
class Trie {
private:
    TrieNode<T>* root_;

    void insertHelper(TrieNode<T>* node, const ArraySequence<T>& key, size_t index) {
        if (index == key.Size()) {
            node->isEnd = true;
            node->count++;
            return;
        }
        T current = key.Get(index);
        TrieNode<T>* child = node->findChild(current);
        if (!child) {
            child = new TrieNode<T>();
            node->addChild(current, child);
        }
        insertHelper(child, key, index + 1);
    }

    void collectAll(TrieNode<T>* node, ArraySequence<T>& current, ArraySequence<Pair<ArraySequence<T>, int>>& results) {
        if (node->isEnd) {
            results.Append(Pair<ArraySequence<T>, int>(current, node->count));
        }
        for (size_t i = 0; i < node->children.Size(); ++i) {
            current.Append(node->children.Get(i).first);
            collectAll(node->children.Get(i).second, current, results);
            current.RemoveAt(current.Size() - 1);
        }
    }

    void sortResults(ArraySequence<Pair<ArraySequence<T>, int>>& results) {
        size_t n = results.Size();
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (results.Get(j).second < results.Get(j + 1).second) {
                    auto tmp = results.Get(j);
                    results.Set(j, results.Get(j + 1));
                    results.Set(j + 1, tmp);
                }
            }
        }
    }

public:
    Trie() : root_(new TrieNode<T>()) {}

    ~Trie() { delete root_; }

    void insert(const ArraySequence<T>& key) {
        insertHelper(root_, key, 0);
    }

    ArraySequence<Pair<ArraySequence<T>, int>> getAllCounts() {
        ArraySequence<Pair<ArraySequence<T>, int>> results;
        ArraySequence<T> current;
        collectAll(root_, current, results);
        return results;
    }

    ArraySequence<Pair<ArraySequence<char>, int>> getFrequentStrings() {
        auto counts = getAllCounts();
        sortResults(counts);
        return counts;
    }
};

#endif