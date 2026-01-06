#ifndef LAZY_SEQUENCE_H
#define LAZY_SEQUENCE_H

#include "ListSequence.h"
#include "Generator.h"
#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>

template <typename T>
class LazySequence {
private:
    std::unique_ptr<ListSequence<T>> memo_;
    std::shared_ptr<Generator<T>> generator_;
    bool is_finite_;
    size_t finite_capacity_;

    void materialize_up_to(size_t index) {
        if (is_finite_ && index >= finite_capacity_) {
            throw std::out_of_range("Index exceeds finite capacity");
        }
        while (memo_->Size() <= index) {
            if (!generator_) {
                throw std::out_of_range("No generator");
            }
            if (!generator_->HasNext()) {
                if (is_finite_) break;
                throw std::out_of_range("Generator exhausted");
            }
            T val = generator_->GetNext();
            memo_->Append(val);
        }
    }

public:
    LazySequence()
        : memo_(std::make_unique<ListSequence<T>>())
        , generator_(nullptr)
        , is_finite_(true)
        , finite_capacity_(0) {}

    explicit LazySequence(std::shared_ptr<Generator<T>> gen)
        : memo_(std::make_unique<ListSequence<T>>())
        , generator_(std::move(gen))
        , is_finite_(false)
        , finite_capacity_(0) {}

    LazySequence(T* items, size_t count)
        : memo_(std::make_unique<ListSequence<T>>(items, count))
        , generator_(nullptr)
        , is_finite_(true)
        , finite_capacity_(count) {}

    explicit LazySequence(const ListSequence<T>& seed)
        : memo_(std::make_unique<ListSequence<T>>(seed))
        , generator_(nullptr)
        , is_finite_(true)
        , finite_capacity_(seed.Size()) {}

    LazySequence(const LazySequence& other) 
        : memo_(std::make_unique<ListSequence<T>>(*other.memo_))
        , generator_(other.generator_)
        , is_finite_(other.is_finite_)
        , finite_capacity_(other.finite_capacity_) {}

    LazySequence& operator=(const LazySequence& other) {
        if (this != &other) {
            memo_ = std::make_unique<ListSequence<T>>(*other.memo_);
            generator_ = other.generator_;
            is_finite_ = other.is_finite_;
            finite_capacity_ = other.finite_capacity_;
        }
        return *this;
    }

    LazySequence(LazySequence&& other) noexcept 
    : memo_(std::move(other.memo_))
    , generator_(std::move(other.generator_))
    , is_finite_(other.is_finite_)
    , finite_capacity_(other.finite_capacity_) {}

    LazySequence& operator=(LazySequence&& other) noexcept {
    if (this != &other) {
        memo_ = std::move(other.memo_);
        generator_ = std::move(other.generator_);
        is_finite_ = other.is_finite_;
        finite_capacity_ = other.finite_capacity_;
    }
    return *this;
    }

    T Get(size_t index) const {
        if (is_finite_ && index >= finite_capacity_) {
            throw std::out_of_range("Index out of finite capacity");
        }
        const_cast<LazySequence*>(this)->materialize_up_to(index);
        return memo_->Get(index);
    }
    
    T GetFirst() { return Get(0); }

    T GetLast() {
        if (is_finite_) {
            if (finite_capacity_ == 0) throw std::out_of_range("Empty finite");
            if (memo_->Size() < finite_capacity_) materialize_up_to(finite_capacity_ - 1);
            return memo_->Get(finite_capacity_ - 1);
        } else {
            if (memo_->Size() == 0 && generator_) materialize_up_to(0);
            if (memo_->Size() == 0) throw std::out_of_range("Empty");
            return memo_->GetLast();
        }
    }

    size_t get_size() const { return Size(); }
    size_t get_capacity() const { return finite_capacity(); }

    void set_capacity(size_t cap) {
        if (cap < memo_->Size()) {
            throw std::invalid_argument("New capacity must be >= current size");
        }
        is_finite_ = true;
        finite_capacity_ = cap;
    }

    void set_finite(bool finite, size_t capacity = 0) {
        if (finite && capacity < memo_->Size()) {
            throw std::invalid_argument("Capacity too small");
        }
        is_finite_ = finite;
        finite_capacity_ = finite ? capacity : 0;
    }

    size_t Size() const { return is_finite_ ? finite_capacity_ : memo_->Size(); }
    bool is_finite() const { return is_finite_; }
    size_t finite_capacity() const { return is_finite_ ? finite_capacity_ : 0; }
    size_t GetMaterializedCount() const { return memo_->Size(); }

    std::unique_ptr<LazySequence<T>> concat(const LazySequence<T>& other) const {
        auto self_ptr = std::make_shared<LazySequence<T>>(*this);
        auto other_ptr = std::make_shared<LazySequence<T>>(other);
        auto concat_gen = std::make_shared<ConcatGenerator<T>>(std::move(self_ptr), std::move(other_ptr));
        return std::make_unique<LazySequence<T>>(std::move(concat_gen));
    }

    std::unique_ptr<LazySequence<T>> insert(const LazySequence<T>& to_insert, size_t index) const {
        auto self_ptr = std::make_shared<LazySequence<T>>(*this);
        auto insert_ptr = std::make_shared<LazySequence<T>>(to_insert);
        auto insert_gen = std::make_shared<InsertGenerator<T>>(std::move(self_ptr), std::move(insert_ptr), index);
        return std::make_unique<LazySequence<T>>(std::move(insert_gen));
    }

    template <typename M>
    std::unique_ptr<LazySequence<M>> map(std::function<M(T)> func) const {
        auto self_ptr = std::make_shared<LazySequence<T>>(*this);
        auto map_gen = std::make_shared<MapGenerator<T, M>>(std::move(self_ptr), std::move(func));
        return std::make_unique<LazySequence<M>>(std::move(map_gen));
    }

    template <typename T2>
    ListSequence<std::pair<T, T2>> Zip(const LazySequence<T2>& other) const {
        ZipGenerator<T, T2> zipper(std::make_shared<LazySequence<T>>(*this), std::make_shared<LazySequence<T2>>(other));
        ListSequence<std::pair<T, T2>> result;
        while (zipper.HasNext()) {
            result.Append(zipper.GetNext());
        }
        return result;
    }

    std::unique_ptr<LazySequence<T>> Where(std::function<bool(T)> pred) const {
        auto self_ptr = std::make_shared<LazySequence<T>>(*this);
        auto where_gen = std::make_shared<WhereGenerator<T>>(std::move(self_ptr), std::move(pred));
        return std::make_unique<LazySequence<T>>(std::move(where_gen));
    }

    template <typename T2>
    T2 Reduce(std::function<T2(T2, T)> reucer, T2 initial) const {
        T2 acc = initial;
        size_t i = 0;
        while (true) {
            if (is_finite() && i >= get_capacity()) {
                break;
            }
            try {
                T val = Get(i++);
                acc = reucer(acc, val);
            } catch (const std::out_of_range&) {
                break;
            }
        }
        return acc;
    }

    void Print() const {
        std::cout << "LazySeq (finite=" << is_finite_
                  << ", cap=" << (is_finite_ ? std::to_string(finite_capacity_) : "inf")
                  << ", mat=" << memo_->Size() << "): ";
        for (size_t i = 0; i < memo_->Size(); ++i) {
            std::cout << memo_->Get(i) << " ";
        }
        std::cout << "\n";
    }

};

#endif