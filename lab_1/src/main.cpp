#include <iostream>
#include <memory>
#include "sequence/LazySequence.h"
#include "sequence/MedianLazySequence.h"
#include "sequence/Generator.h"

int main() {
    std::cout << "=== Median Lazy Sequence Demo ===\n";
    std::cout << "1. Natural\n";
    std::cout << "2. f(x) = 2*x + 5\n";
    std::cout << "3. Sin\n";
    std::cout << "Enter the coman:";
    int choice;
    std::cin >> choice;

    std::shared_ptr<Generator<double>> gen;

    switch (choice) {
        case 1: gen = std::make_shared<NaturalNumbersDoubleGenerator>(); break;
        case 2: gen = std::make_shared<LinearFunctionGenerator>(); break;
        case 3: gen = std::make_shared<SineSequenceGenerator>(); break;
        default:
            std::cerr << "Invalid choice!\n";
            return 1;
    }

    MedianLazySequence<double> seq(gen);

    for (size_t i = 0; i < 10; ++i) {
        double val = seq.Get(i);
        double med = seq.get_current_median();
        std::cout << i << ": Value = " << val << " | Meian = " << med << "\n";
    }

    return 0;
}