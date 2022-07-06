#pragma once
#include "knapsack.hpp"

#include <queue>

namespace kp {

auto bellman(Instance const& kp) -> AssignmentSolution
{
    weight_t const row_size = kp.capacity + 1;
    std::vector<profit_t> max_profit(row_size, 0);
    std::vector<bool> indicator(row_size * kp.n, false);

    std::uint32_t cell = 0;
    for (auto [profit, weight] : kp.items) {
        for (weight_t e = row_size - weight, d = kp.capacity; e; --d) {
            auto new_profit = profit + max_profit[--e];
            if (new_profit > max_profit[d]) {
                max_profit[d] = new_profit;
                indicator[cell + d] = true;
            }
        }
        cell += row_size;
    }

    --cell;
    AssignmentSolution solution(kp.n);
    solution.profit = max_profit[kp.capacity];
    item_id i = kp.n;
    for (auto [profit, weight] : kp.items | vs::reverse) {
        if (solution.assignment[--i] = indicator[cell]) {
            solution.weight += weight;
            cell -= row_size + weight;
        } else
            cell -= row_size;
    }

    return solution;
}


auto bb_best_first_search(Instance const& kp) -> AssignmentSolution
{
    struct Node
    {
        AssignmentSolution state;
        item_id item; // next item to branch; the fixed variables are from 0 to item - 1
        profit_t upperbound;

        /* comparaison needed for std::priority_queue to retrieve the most promising node
        (i.e. with the greatest upperbound) */
        bool operator<(Node const& rhs) const { return upperbound < rhs.upperbound; }
    };

    // updates the upperbound of the given node in quasi-linear time
    auto update_ub = [Σ = partial_sum(kp.items), W = kp.capacity](Node& u) {
        return u.upperbound = u.state.profit +
            static_cast<profit_t>(lkp(Σ, u.item, W - u.state.weight));
    };

    // an optimisation to avoid unecessary branches, see (*)
    auto min_weights = minimum_weight_vec(kp);

    std::priority_queue<Node> nodes;
    AssignmentSolution best(kp.n); // the best visited solution is a global lowerbound

    Node initial_node {best, 0, 0};
    update_ub(initial_node);
    nodes.push(std::move(initial_node));

    while (not nodes.empty()) {
        auto node = nodes.top();
        nodes.pop();

        if (node.upperbound <= best.profit or node.item == kp.n) continue;
        auto i = node.item++;

        // Branch x_i = 0 ; "don't pack the item"
        if (node.item < kp.n and update_ub(node) > best.profit) nodes.push(node);

        // Branch x_i = 1 ; "pack the item"
        auto const& [p, w] = kp.items[i];
        node.state.weight += w;
        if (node.state.weight <= kp.capacity) {
            node.state.profit += p;
            node.state.assignment[i] = true;
            if (node.state.profit > best.profit) best = node.state;
            if (kp.capacity - node.state.weight >= min_weights[node.item] and // (*)
                node.item < kp.n and update_ub(node) > best.profit)
                nodes.push(node);
        }
    }

    return best;
}

}; 