#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "Sequence.h"
#include <cstddef>
#include <stdexcept>

template <typename T>
class ArraySequence : public Sequence<T> {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void resize() {
        size_t newCap = (capacity_ == 0) ? 1 : capacity_ * 2;
        T* newData = new T[newCap];
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCap;
    }

    void checkIndex(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
    }

public:
    using size_type = size_t;

    ArraySequence() : data_(nullptr), size_(0), capacity_(0) {}

    explicit ArraySequence(size_type n) : size_(n), capacity_(n) {
        data_ = new T[capacity_]();
    }

    ArraySequence(const T* items, size_type count) : size_(count), capacity_(count) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = items[i];
        }
    }

    ArraySequence(const ArraySequence& other) : size_(other.size_), capacity_(other.capacity_) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    ArraySequence& operator=(const ArraySequence& other) {
        if (this == &other) return *this;
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }

    bool operator==(const ArraySequence& other) const {
        if (size_ != other.size_) return false;
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) return false;
        }
        return true;
    }

    ~ArraySequence() {
        delete[] data_;
    }

    T GetFirst() const override {
        if (IsEmpty()) throw std::out_of_range("Sequence is empty");
        return data_[0];
    }

    T GetLast() const override {
        if (IsEmpty()) throw std::out_of_range("Sequence is empty");
        return data_[size_ - 1];
    }

    T Get(size_type index) const override {
        checkIndex(index);
        return data_[index];
    }

    T& Get(size_type index) {
        checkIndex(index);
        return data_[index];
    }

    void Set(size_type index, const T& value) override {
        checkIndex(index);
        data_[index] = value;
    }

    void Append(const T& item) override {
        if (size_ == capacity_) resize();
        data_[size_++] = item;
    }

    void Prepend(const T& item) override {
        if (size_ == capacity_) resize();

        for (size_t i = size_; i > 0; --i) {
            data_[i] = data_[i - 1];
        }
        data_[0] = item;
        ++size_;
    }

    void InsertAt(size_type index, const T& item) override {
        if (index > size_) {
            throw std::out_of_range("InsertAt: index out of range");
        }
        if (size_ == capacity_) resize();

        for (size_t i = size_; i > index; --i) {
            data_[i] = data_[i - 1];
        }
        data_[index] = item;
        ++size_;
    }

    void RemoveAt(size_type index) override {
        checkIndex(index);
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
    }

    size_type Size() const override {
        return size_;
    }

    bool IsEmpty() const override {
        return size_ == 0;
    }

    void Clear() override {
        size_ = 0;
    }

    T& operator[](size_type index) {
        checkIndex(index);
        return data_[index];
    }

    const T& operator[](size_type index) const {
        checkIndex(index);
        return data_[index];
    }

    void push_back(const T& item) {
        Append(item);
    }
};


template<typename T>
bool operator==(const ArraySequence<T>& a, const ArraySequence<T>& b) {
    if (a.Size() != b.Size()) return false;
    for (size_t i = 0; i < a.Size(); ++i) {
        if (a.Get(i) != b.Get(i)) return false;
    }
    return true;
}


#endif