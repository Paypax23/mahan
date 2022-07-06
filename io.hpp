
#pragma once
#include "knapsack.hpp"
#include "utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace kp {

namespace fs = std::filesystem;

// Output ────────────────────────────────────────────────────────────────────────────────

template <class T>
auto& operator<<(std::ostream& os, std::vector<T> const& v)
{
    os << "[ ";
    std::ranges::copy(v, std::ostream_iterator<T>(os, " "));
    return os << "] ";
}

auto& operator<<(std::ostream& os, Item const& i)
{
    return os << '(' << i.profit << ',' << i.weight << ')';
}

auto& operator<<(std::ostream& os, Instance const& kp)
{
    return os << "Items: " << kp.items << "\nCapacity: " << kp.capacity << '\n';
}

auto& operator<<(std::ostream& os, AssignmentSolution const& s)
{
    return os << "Solution: " << s.assignment << "\nProfit: " << s.profit
              << "\nWeight: " << s.weight << '\n';
}

// Input ─────────────────────────────────────────────────────────────────────────────────

auto& operator>>(std::istream& is, Item& i) { return is >> i.profit >> i.weight; }

auto read(fs::path const& filepath) -> Instance
{
    Instance kp;
    std::ifstream file {filepath};
    file >> kp.n >> kp.capacity;
    kp.items.reserve(kp.n);
    std::copy_n(std::istream_iterator<Item>(file), kp.n, utils::back_emplacer(kp.items));
    return kp;
}

struct SolvedInstance
{
    std::string name;
    Instance instance;
    profit_t solution;
};

/* Reads a folder of instances, skips instances without associated solution files. */
auto read_instances(fs::path const& dir) -> std::vector<SolvedInstance>
{
    std::vector<SolvedInstance> instances;
    for (auto& de : fs::directory_iterator {dir}) {
        auto instance = de.path();
        if (instance.extension() == ".kp") {
            auto solution = dir / "solution" / instance.stem().concat(".sol");
            if (!fs::exists(solution)) {
                std::cout << "[Error] The solution file of the instance "
                          << instance.filename() << " doesn't exist. Skipping it.\n";
                continue;
            }
            std::ifstream is {solution};
            instances.emplace_back(instance.stem().string(), read(instance),
                *std::istream_iterator<profit_t>(is));
        }
    }
    return instances;
}

} 