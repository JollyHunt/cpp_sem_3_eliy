#include <functional>
#include <iostream>
#include <string>
#include <memory>
#include "../include/sequence/LazySequence.h"
#include "../include/sequence/Generator.h"

int main() {
    auto test_seq = LazySequence<int>();
    
    test_seq.Print();
}
