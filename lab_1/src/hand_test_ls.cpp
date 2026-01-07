#include <iostream>
#include <memory>
#include <string>
#include "stream/Stream.h"

int main() {
    std::cout << "Enter int: ";
    auto seq1 = Stream<int>::from_cin();
    std::cout << "Rea from cin: ";
    Stream<int>::print_to_cout(seq1);

    Stream<int>::print_to_file(seq1, "../files/test_ints.txt");
    std::cout << "Save to ../files/test_ints.txt\n";

    auto seq2 = Stream<int>::from_file("../files/test_ints.txt");
    std::cout << "Rea from text file: ";
    Stream<int>::print_to_cout(seq2);

    Stream<int>::print_to_binary(seq1, "../files/test_ints.bin");
    std::cout << "Save to ../files/test_ints.bin\n";

    auto seq3 = Stream<int>::from_binary("../files/test_ints.bin");
    std::cout << "Rea from binary file: ";
    Stream<int>::print_to_cout(seq3);

    std::cout << "Enter words: ";
    auto str_seq = Stream<std::string>::from_cin();
    std::cout << "Rea strings: ";
    Stream<std::string>::print_to_cout(str_seq);
    Stream<std::string>::print_to_file(str_seq, "../files/test_strings.txt");

    auto str_seq2 = Stream<std::string>::from_file("../files/test_strings.txt");
    std::cout << "Rea strings from file: ";
    Stream<std::string>::print_to_cout(str_seq2);

    return 0;
}