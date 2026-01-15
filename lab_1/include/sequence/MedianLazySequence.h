#ifndef MEDIAN_LAZY_SEQUENCE_H
#define MEDIAN_LAZY_SEQUENCE_H

#include "LazySequence.h"
#include "bintree/BinTree.h"
#include <memory>

template<typename T>
class MedianLazySequence : public LazySequence<T> {
private:
    std::unique_ptr<BinTree<T>> sorted_memo_;

protected:
    void materialize_up_to(size_t index) override {
        size_t current_size = this->memo_->Size();

        if (this->is_finite() && index >= this->finite_capacity()) {
            throw std::out_of_range("Index exceeds finite capacity");
        }

        while (this->memo_->Size() <= index) {
            if (!this->generator_) {
                throw std::out_of_range("No generator");
            }
            if (!this->generator_->HasNext()) {
                if (this->is_finite()) break;
                throw std::out_of_range("Generator exhausted");
            }

            T val = this->generator_->GetNext();
            this->memo_->Append(val);
            sorted_memo_->insert(val);
        }
    }

public:
    explicit MedianLazySequence(std::shared_ptr<Generator<T>> gen)
        : LazySequence<T>(std::move(gen)),
          sorted_memo_(std::make_unique<BinTree<T>>()) {
    }

    T get_current_median() const {
        if (sorted_memo_->empty()) {
            throw std::runtime_error("No elements to compute median");
        }
        return sorted_memo_->median();
    }

    using LazySequence<T>::Get;
};

#endif