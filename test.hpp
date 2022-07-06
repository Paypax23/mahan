#pragma once
#include "algorithms.hpp"
#include "io.hpp"
#include "knapsack.hpp"

namespace kp {

void test_directory(fs::path dir)
{
    std::cout << "Testing instances from " << dir << '\n';

    using algorithm_t =
        std::tuple<std::string, bool, std::function<AssignmentSolution(Instance const&)>>;

    std::array<algorithm_t, 2> algorithms {
        {{"Bellman (Dynamic Programming)", false, bellman},
            {"Primal Branch and Bound (Best First Search)", true, bb_best_first_search}}};

    auto instances = read_instances(dir);

    for (auto const& [desc, to_sort, solver] : algorithms) {
        std::cout << '\n' << desc << '\n';
        for (auto [name, kp, value] : instances) {
            std::cout << name << " : ";
            AssignmentSolution solution(kp.n);
            {
                utils::Timer timer;
                if (to_sort) kp.sort();
                solution = solver(kp);
            }
            if (solution.profit != value or check_solution(kp, solution)) {
                std::cout << "[Error] Fail on instance " << name << '\n';
            }
        }
    }
}

} 