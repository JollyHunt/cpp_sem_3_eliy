#ifndef GENERATOR_H
#define GENERATOR_H

#include <memory>
#include <optional>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <string>
#include <cmath>

template <typename T>
class LazySequence;

template <typename T>
class Generator {
public:
    virtual ~Generator() = default;
    virtual T GetNext() = 0;
    virtual bool HasNext() const { return true; }
    virtual std::optional<T> TryGetNext() {
        if (!HasNext()) return std::nullopt;
        return GetNext();
    }
};

template <typename T>
class WhereGenerator : public Generator<T> {
private:
    std::shared_ptr<LazySequence<T>> source_;
    std::function<bool(T)> predicate_;
    size_t source_index_ = 0;

public:
    WhereGenerator(std::shared_ptr<LazySequence<T>> source, std::function<bool(T)> pred)
        : source_(std::move(source)), predicate_(std::move(pred)) {}

    T GetNext() override {
        while (true) {
            if (source_->is_finite() && source_index_ >= source_->get_capacity()) {
                throw std::out_of_range("Filter exhausted");
            }
            T val = source_->Get(source_index_++);
            if (predicate_(val)) {
                return val;
            }
        }
    }

    bool HasNext() const override {
        return !source_->is_finite() || source_index_ < source_->get_capacity();
    }
};

template <typename T>
class NaturalNumbersGenerator : public Generator<T> {
private:
    T current_ = T{0};

public:
    T GetNext() override {
        return current_++;
    }

    bool HasNext() const override {
        return true;
    }
};

template <typename T>
class ArithmeticProgressionGenerator : public Generator<T> {
private:
    T current_;
    T step_;

public:
    ArithmeticProgressionGenerator(T start, T step) : current_(start), step_(step) {}

    T GetNext() override {
        T val = current_;
        current_ += step_;
        return val;
    }

    bool HasNext() const override {
        return true;
    }
};

class StringSequenceGenerator : public Generator<std::string> {
private:
    size_t counter_ = 0;

public:
    std::string GetNext() override {
        return "s" + std::to_string(counter_++);
    }

    bool HasNext() const override {
        return true;
    }
};

template <typename T, typename M>
class MapGenerator : public Generator<M> {
private:
    std::shared_ptr<LazySequence<T>> source_;
    std::function<M(T)> mapper_;
    size_t index_ = 0;

public:
    MapGenerator(std::shared_ptr<LazySequence<T>> source, std::function<M(T)> mapper)
        : source_(std::move(source)), mapper_(std::move(mapper)) {}

    M GetNext() override {
        if (source_->is_finite() && index_ >= source_->get_capacity()) {
            throw std::out_of_range("Generator exhausted");
        }
        T val = source_->Get(index_++);
        return mapper_(val);
    }

    bool HasNext() const override {
        if (source_->is_finite()) {
            return index_ < source_->get_capacity();
        }
        return true;
    }
};

template <typename T>
class ConcatGenerator : public Generator<T> {
private:
    std::shared_ptr<LazySequence<T>> first_;
    std::shared_ptr<LazySequence<T>> second_;
    size_t current_index_ = 0;
    size_t first_size_;

public:
    ConcatGenerator(std::shared_ptr<LazySequence<T>> first, std::shared_ptr<LazySequence<T>> second)
        : first_(std::move(first)), second_(std::move(second)) {
        first_size_ = first_->is_finite() ? first_->get_capacity() : 0;
    }

    T GetNext() override {
        if (first_->is_finite()) {
            if (current_index_ < first_size_) {
                return first_->Get(current_index_++);
            } else {
                size_t second_idx = current_index_ - first_size_;
                current_index_++;
                return second_->Get(second_idx);
            }
        } else {
            return first_->Get(current_index_++);
        }
    }

    bool HasNext() const override {
        if (first_->is_finite() && second_->is_finite()) {
            size_t total = first_size_ + second_->get_capacity();
            return current_index_ < total;
        }
        return true;
    }
};

template <typename T>
class InsertGenerator : public Generator<T> {
private:
    std::shared_ptr<LazySequence<T>> main_;
    std::shared_ptr<LazySequence<T>> insert_;
    size_t insert_at_;
    size_t current_index_ = 0;
    size_t main_size_;
    size_t insert_size_;

public:
    InsertGenerator(std::shared_ptr<LazySequence<T>> main, std::shared_ptr<LazySequence<T>> insert, size_t at)
        : main_(std::move(main)), insert_(std::move(insert)), insert_at_(at) {
        if (!insert_->is_finite()) {
            throw std::invalid_argument("Cannot insert infinite sequence");
        }
        main_size_ = main_->is_finite() ? main_->get_capacity() : 0;
        insert_size_ = insert_->get_capacity();
    }

    T GetNext() override {
        if (current_index_ < insert_at_) {
            if (main_->is_finite() && current_index_ >= main_size_) {
                throw std::out_of_range("Main exhausted before insert point");
            }
            return main_->Get(current_index_++);
        } else if (current_index_ < insert_at_ + insert_size_) {
            size_t idx_in_insert = current_index_ - insert_at_;
            current_index_++;
            return insert_->Get(idx_in_insert);
        } else {
            size_t idx_in_main = current_index_ - insert_size_;
            if (main_->is_finite() && idx_in_main >= main_size_) {
                throw std::out_of_range("Main exhausted");
            }
            current_index_++;
            return main_->Get(idx_in_main);
        }
    }

    bool HasNext() const override {
        if (main_->is_finite()) {
            size_t total_size = main_size_ + insert_size_;
            return current_index_ < total_size;
        }
        return true;
    }
};

template <typename T, typename T2>
class ZipGenerator : public Generator<std::pair<T, T2>> {
private:
    std::shared_ptr<LazySequence<T>> first_;
    std::shared_ptr<LazySequence<T2>> second_;
    size_t current_index_ = 0;

public:
    ZipGenerator(std::shared_ptr<LazySequence<T>> first, std::shared_ptr<LazySequence<T2>> second)
        : first_(std::move(first)), second_(std::move(second)) {}

    std::pair<T, T2> GetNext() override {
        if (first_->is_finite() && current_index_ >= first_->get_capacity()) {
            throw std::out_of_range("First sequence exhausted");
        }
        if (second_->is_finite() && current_index_ >= second_->get_capacity()) {
            throw std::out_of_range("Second sequence exhausted");
        }
        
        T val1 = first_->Get(current_index_);
        T2 val2 = second_->Get(current_index_);
        current_index_++;
        return std::make_pair(val1, val2);
    }

    bool HasNext() const override {
        if (first_->is_finite() && current_index_ >= first_->get_capacity()) {
            return false;
        }
        if (second_->is_finite() && current_index_ >= second_->get_capacity()) {
            return false;
        }
        return true;
    }
};


class NaturalNumbersDoubleGenerator : public Generator<double> {
private:
    double current_ = 1.0;

public:
    double GetNext() override {
        return current_++;
    }

    bool HasNext() const override {
        return true;
    }
};

class LinearFunctionGenerator : public Generator<double> {
private:
    int x_ = 0;

public:
    double GetNext() override {
        double val = 2.0 * x_ + 5.0;
        x_++;
        return val;
    }

    bool HasNext() const override {
        return true;
    }
};

class SineSequenceGenerator : public Generator<double> {
private:
    int n_ = 0;
    static constexpr double STEP = M_PI / 12.0;

public:
    double GetNext() override {
        double x = n_ * STEP;
        n_++;
        return std::sin(x);
    }

    bool HasNext() const override {
        return true;
    }
};

#endif