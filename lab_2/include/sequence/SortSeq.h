#ifndef SORTED_SEQUENCE_H
#define SORTED_SEQUENCE_H

#include "ListSequence.h"
#include <algorithm>

template <typename TElement>
class SortedSequence : public Sequence<TElement> {
private:
    ListSequence<TElement> data_;

public:
    using size_type = typename Sequence<TElement>::size_type;

    SortedSequence() = default;

    SortedSequence(const TElement* items, size_type count) : SortedSequence() {
        for (size_type i = 0; i < count; ++i) Add(items[i]);
    }

    explicit SortedSequence(const Sequence<TElement>* other) : SortedSequence() {
        if (!other) return;
        for (size_t i = 0; i < other->Size(); ++i) Add(other->Get(i));
    }

    SortedSequence(const SortedSequence& other) : data_(other.data_) {}

    SortedSequence& operator=(const SortedSequence&) = delete;

    ~SortedSequence() = default;

    int GetLength() const { return static_cast<int>(data_.Size()); }
    int GetIsEmpty() const { return data_.IsEmpty() ? 1 : 0; }

    TElement Get(int index) const {
        if (index < 0 || static_cast<size_type>(index) >= data_.Size())
            throw std::out_of_range("Index out of range");
        return data_.Get(static_cast<size_type>(index));
    }

    TElement GetFirst() const {
        if (data_.IsEmpty()) throw std::out_of_range("Sequence is empty");
        return data_.Get(0);
    }

    TElement GetLast() const {
        if (data_.IsEmpty()) throw std::out_of_range("Sequence is empty");
        return data_.Get(data_.Size() - 1);
    }

    int IndexOf(TElement element) const {
        for (size_type i = 0; i < data_.Size(); ++i)
            if (data_.Get(i) == element) return static_cast<int>(i);
        return -1;
    }

    SortedSequence<TElement> GetSubsequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < startIndex || static_cast<size_type>(endIndex) > data_.Size())
            throw std::out_of_range("Invalid subsequence range");

        SortedSequence<TElement> result;
        for (int i = startIndex; i <= endIndex; ++i)
            result.Add(data_.Get(static_cast<size_type>(i)));
        return result;
    }

    void Add(TElement element) {
        size_type left = 0, right = data_.Size();
        while (left < right) {
            size_type mid = left + (right - left) / 2;
            if (data_.Get(mid) < element) left = mid + 1;
            else right = mid;
        }
        data_.InsertAt(left, element);
    }

    TElement Get(size_type index) const override { return Get(static_cast<int>(index)); }

    void Set(size_type index, const TElement& value) override {
        throw std::runtime_error("Set not supported in SortedSequence");
    }

    void Append(const TElement& item) override {
        throw std::runtime_error("Append not supported in SortedSequence");
    }

    void Prepend(const TElement& item) override {
        throw std::runtime_error("Prepend not supported in SortedSequence");
    }

    void InsertAt(size_type index, const TElement& item) override {
        throw std::runtime_error("InsertAt not supported in SortedSequence");
    }

    void RemoveAt(size_type index) override { data_.RemoveAt(index); }

    size_type Size() const override { return data_.Size(); }

    bool IsEmpty() const override { return data_.IsEmpty(); }

    void Clear() override { data_.Clear(); }
};

#endif