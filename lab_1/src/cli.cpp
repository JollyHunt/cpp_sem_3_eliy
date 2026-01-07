#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include "../include/sequence/LazySequence.h"
#include "../include/sequence/Generator.h"

int main() {
    int cmd;
    while (true) {
        std::string hepl_win = "1-natural\n 2-arith\n 3-strings\n 4-concat\n 5-map\n 6-where\n 7-zip\n 0-exit\n ----------\n> ";
        std::cout << hepl_win;
        std::cin >> cmd;
        switch (cmd) {
            case 0:
                return 0;
            case 1: {
                std::cout << "Enter count: ";
                size_t n; std::cin >> n;
                auto g = std::make_shared<NaturalNumbersGenerator<int>>();
                LazySequence<int> s(g);
                for (size_t i = 0; i < n; ++i) std::cout << s.Get(i) << " ";
                std::cout << "\n";
                break;
            }
            case 2: {
                std::cout << "Enter count, start, step: ";
                size_t n; double a, d; std::cin >> n >> a >> d;
                auto g = std::make_shared<ArithmeticProgressionGenerator<double>>(a, d);
                LazySequence<double> s(g);
                for (size_t i = 0; i < n; ++i) std::cout << s.Get(i) << " ";
                std::cout << "\n";
                break;
            }
            case 3: {
                std::cout << "Enter count: ";
                size_t n; std::cin >> n;
                auto g = std::make_shared<StringSequenceGenerator>();
                LazySequence<std::string> s(g);
                for (size_t i = 0; i < n; ++i) std::cout << s.Get(i) << " ";
                std::cout << "\n";
                break;
            }
            case 4: {
                std::cout << "Enter 4 integers (a b c d) for two sequences [a,b] and [c,d]: ";
                int a, b, c, d; std::cin >> a >> b >> c >> d;
                int x[] = {a, b}; int y[] = {c, d};
                LazySequence<int> s1(x, 2), s2(y, 2);
                auto r = s1.concat(s2);
                std::cout << r->Get(0) << " " << r->Get(1) << " " << r->Get(2) << " " << r->Get(3) << "\n";
                break;
            }
            case 5: {
                std::cout << "Enter 3 integers (x y z) to square each: ";
                int a, b, c; std::cin >> a >> b >> c;
                int x[] = {a, b, c};
                LazySequence<int> s(x, 3);
                auto r = s.map<int>([](int v) { return v * v; });
                std::cout << r->Get(0) << " " << r->Get(1) << " " << r->Get(2) << "\n";
                break;
            }
            case 6: {
                std::cout << "Enter count of odd numbers to show: ";
                size_t n; std::cin >> n;
                auto g = std::make_shared<NaturalNumbersGenerator<int>>();
                LazySequence<int> s(g);
                auto r = s.Where([](int v) { return v % 2 == 1; });
                for (size_t i = 0; i < n; ++i) std::cout << r->Get(i) << " ";
                std::cout << "\n";
                break;
            }
            case 7: {
                std::cout << "Enter 3 integers and 3 strings (a b c s1 s2 s3): ";
                int a1, a2, a3; std::cin >> a1 >> a2 >> a3;
                std::string b1, b2, b3; std::cin >> b1 >> b2 >> b3;
                int xa[] = {a1, a2, a3};
                std::string xb[] = {b1, b2, b3};
                LazySequence<int> s1(xa, 3);
                LazySequence<std::string> s2(xb, 3);
                auto z = s1.Zip(s2);
                for (size_t i = 0; i < 3; ++i)
                    std::cout << "(" << z->Get(i).first << " " << z->Get(i).second << ") ";
                std::cout << "\n";
                break;
            }
            default:
                std::cout << "Invalid command\n";
        }
    }
}