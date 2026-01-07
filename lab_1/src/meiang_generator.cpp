#include "sequence/Generator.h"
#include "sequence/LazySequence.h"

MedianGenerator::MedianGenerator() 
    : sorted_data_(std::make_unique<LazySequence<double>>()) {}

void MedianGenerator::insert_sorted(double value) {
    double new_item[] = {value};
    LazySequence<double> new_seq(new_item, 1);
    
    size_t insert_pos = 0;
    size_t mat_count = sorted_data_->GetMaterializedCount();
    for (size_t i = 0; i < mat_count; ++i) {
        if (sorted_data_->Get(i) > value) break;
        insert_pos++;
    }
    
    auto temp = sorted_data_->insert(new_seq, insert_pos);
    sorted_data_ = std::move(temp);
    count_++;
}

double MedianGenerator::calculate_median() const {
    if (count_ == 0) return 0.0;
    if (count_ % 2 == 1) {
        return sorted_data_->Get(count_ / 2);
    } else {
        size_t mid1 = count_ / 2 - 1;
        size_t mid2 = count_ / 2;
        return (sorted_data_->Get(mid1) + sorted_data_->Get(mid2)) / 2.0;
    }
}

double SineMedianGenerator::GetNext() {
    double value = std::sin(angle_);
    angle_ += STEP;
    insert_sorted(value);
    return calculate_median();
}

double CosineMedianGenerator::GetNext() {
    double value = std::cos(angle_);
    angle_ += STEP;
    insert_sorted(value);
    return calculate_median();
}

double LinearMedianGenerator::GetNext() {
    double value = 3.0 * x_ - 7.0;
    x_ += 1.0;
    insert_sorted(value);
    return calculate_median();
}

FunctionalMedianGenerator::FunctionalMedianGenerator(std::function<double()> gen) 
    : generator_(std::move(gen)) {}

double FunctionalMedianGenerator::GetNext() {
    double value = generator_();
    insert_sorted(value);
    return calculate_median();
}