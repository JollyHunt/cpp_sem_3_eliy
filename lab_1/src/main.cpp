#include <iostream>
#include <memory>
#include <random>
#include "sequence/Generator.h"
#include "stream/Stream.h"

void test_generator(const std::string& name, std::shared_ptr<MedianGenerator> gen, size_t count = 10) {
    std::cout << "\n-----------" << name << "--------------\n";
    LazySequence<double> seq(gen);
    for (size_t i = 0; i < count; ++i) {
        std::cout << "Step " << (i+1) << ": meian = " << seq.Get(i) << "\n";
    }
    std::cout << "--------------------------------------------\n";
}

int main() {
    test_generator("Sine Median", std::make_shared<SineMedianGenerator>());

    test_generator("Cosine Median", std::make_shared<CosineMedianGenerator>());

    test_generator("Linear Median (3x-7)", std::make_shared<LinearMedianGenerator>());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);
    auto random_func = [dist = dist, gen = std::move(gen)]() mutable { return dist(gen); };
    test_generator("Random Median", std::make_shared<FunctionalMedianGenerator>(random_func));

    auto sine_gen = std::make_shared<SineMedianGenerator>();
    auto save_seq = std::make_unique<LazySequence<double>>();
    
    for (int i = 0; i < 20; ++i) {
        double median = sine_gen->GetNext();
        
        double item[] = {median};
        LazySequence<double> single(item, 1);
        
        auto temp = save_seq->insert(single, save_seq->Size());
        *save_seq = *temp;
    }
    
    Stream<double>::print_to_file(save_seq, "../files/sine_medians.txt");
    return 0;
}