#include <iostream>
#include <chrono>
#include <string>
#include "FrequentSubstrings.h"

void printMenu() {
    std::cout << "\n1. Custom\n2. STL\n3. Compare\n4. Exit\n> ";
}

int main() {
    while (true) {
        printMenu();
        int choice;
        std::cin >> choice;

        if (choice == 4) break;

        std::string s;
        int lmin, lmax;

        std::cout << "s: "; std::cin.ignore(); std::getline(std::cin, s);
        std::cout << "lmin: "; std::cin >> lmin;
        std::cout << "lmax: "; std::cin >> lmax;

        switch (choice) {
            case 1: {
                auto start = std::chrono::high_resolution_clock::now();
                auto res = findFrequentSubstringsCustom(s, lmin, lmax);
                auto end = std::chrono::high_resolution_clock::now();

                for (const auto& p : res) std::cout << p.first << ": " << p.second << "\n";

                auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                std::cout << "Time: " << dur.count() << " μs\n";
                break;
            }
            case 2: {
                auto start = std::chrono::high_resolution_clock::now();
                auto res = findFrequentSubstringsSTL(s, lmin, lmax);
                auto end = std::chrono::high_resolution_clock::now();

                for (const auto& p : res) std::cout << p.first << ": " << p.second << "\n";

                auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                std::cout << "Time: " << dur.count() << " μs\n";
                break;
            }
            case 3: {
                auto start1 = std::chrono::high_resolution_clock::now();
                auto res1 = findFrequentSubstringsCustom(s, lmin, lmax);
                auto end1 = std::chrono::high_resolution_clock::now();

                auto start2 = std::chrono::high_resolution_clock::now();
                auto res2 = findFrequentSubstringsSTL(s, lmin, lmax);
                auto end2 = std::chrono::high_resolution_clock::now();

                auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
                auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();

                std::cout << "Custom: " << t1 << " μs\n";
                std::cout << "STL: " << t2 << " μs\n";
                break;
            }
            default:
                std::cout << "Invalid\n";
        }
    }

    return 0;
}