// Copyright 2026 NNTU-CS
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include "tree.h"

std::vector<char> makeAlphabet(int n) {
    std::vector<char> symbols;
    symbols.reserve(n);
    for (int i = 0; i < n; ++i) {
        symbols.push_back(static_cast<char>('A' + i));
    }
    return symbols;
}

long long factorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

int main() {
    std::cout << "Examples\n";
    std::vector<char> demo = { '1', '2', '3', '4' };
    PMTree tree(demo);

    std::cout << "All permutations:\n";
    for (const auto& p : getAllPerms(tree)) {
        for (char c : p) std::cout << c;
        std::cout << ' ';
    }
    std::cout << "\n\n";

    auto p1 = getPerm1(tree, 3);
    std::cout << "getPerm1(3): ";
    for (char c : p1) std::cout << c;
    std::cout << '\n';

    auto p2 = getPerm2(tree, 20);
    std::cout << "getPerm2(20): ";
    for (char c : p2) std::cout << c;
    std::cout << '\n';

    auto p3 = getPerm2(tree, 26);
    std::cout << "getPerm2(26): " << (p3.empty() ? "null vector" : "?") << '\n';
    std::cout << "\nExperiment (n = 1..10):\n";
    const int max_n = 10;
    std::random_device rd;
    std::mt19937 gen(rd());
    system("mkdir result");
    std::ofstream csv("result/results.csv");
    csv << "n,getAllPerms,getPerm1,getPerm2\n";

    for (int n = 1; n <= max_n; ++n) {
        std::vector<char> symbols = makeAlphabet(n);
        PMTree local_tree(symbols);

        auto t0 = std::chrono::high_resolution_clock::now();
        volatile auto all = getAllPerms(local_tree);
        (void)all;
        auto t1 = std::chrono::high_resolution_clock::now();
        double time_all = std::chrono::duration<double, std::micro>(t1 - t0).count();

        long long total_perms = factorial(n);
        std::uniform_int_distribution<long long> dist(1, total_perms);
        long long num = dist(gen);

        t0 = std::chrono::high_resolution_clock::now();
        volatile auto r1 = getPerm1(local_tree, static_cast<int>(num));
        (void)r1;
        t1 = std::chrono::high_resolution_clock::now();
        double time1 = std::chrono::duration<double, std::micro>(t1 - t0).count();

        t0 = std::chrono::high_resolution_clock::now();
        volatile auto r2 = getPerm2(local_tree, static_cast<int>(num));
        (void)r2;
        t1 = std::chrono::high_resolution_clock::now();
        double time2 = std::chrono::duration<double, std::micro>(t1 - t0).count();

        csv << n << "," << time_all << "," << time1 << "," << time2 << "\n";
        std::cout << "n=" << n << " getAllPerms=" << time_all
            << " mks, getPerm1=" << time1
            << " mks, getPerm2=" << time2 << " mks\n";
    }
    csv.close();

    return 0;
}
