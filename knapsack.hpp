
#pragma once
#include "utils.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

namespace kp {

namespace rs = std::ranges;
namespace vs = std::views;

using profit_t = std::uint32_t;
using weight_t = std::uint32_t;
using item_id = std::uint32_t;

struct Item
{
    profit_t profit;
    weight_t weight;

    float efficiency() const { return static_cast<float>(profit) / weight; }

    auto& operator+=(Item const& i)
    {
        profit += i.profit;
        weight += i.weight;
        return *this;
    }

    auto& operator-=(Item const& i)
    {
        profit -= i.profit;
        weight -= i.weight;
        return *this;
    }
};

Item operator+(Item lhs, Item const& rhs) { return lhs += rhs; }
Item operator-(Item lhs, Item const& rhs) { return lhs -= rhs; }

using item_vec = std::vector<Item>;
using item_it = item_vec::iterator;
using item_cit = item_vec::const_iterator;

struct Instance
{
    item_vec items;
    item_id n; // numbers of items, i.e. == size(items)
    weight_t capacity;

    void sort() { rs::sort(items, std::greater<>(), &Item::efficiency); }
};

struct AssignmentSolution
{
    std::vector<bool> assignment;
    profit_t profit;
    weight_t weight;

    AssignmentSolution(item_id n) : assignment(n, false), profit(0), weight(0) {}
};

struct SetSolution
{
    std::vector<item_id> items;
    profit_t profit;
    weight_t weight;
};


auto split_item(item_cit first, item_cit last, weight_t capacity)
    -> std::tuple<item_cit, Item>
{
    Item total {0, 0};
    auto pack = [&](auto item) {
        if (total.weight > capacity) return true; // item can't fit
        total += item;
        capacity -= item.weight; // 'capacity' is used as a residual capacity
        return false;
    };
    auto split = std::find_if(first, last, pack);
    return {split, total};
}

/* Returns a vector Σ such as Σ[i + 1] = sum of items 0 to i. For practical reason, starts
with (0, 0); care about that offset. Used to find the LP relaxation's profit in
quasi-linear time thanks to a dichotomic search.  */
auto partial_sum(item_cit first, item_cit last) -> item_vec
{
    item_vec out;
    out.reserve(std::distance(first, last) + 1);
    out.emplace_back(0, 0);
    std::partial_sum(first, last, utils::back_emplacer(out));
    return out;
}

auto partial_sum(item_vec const& items) { return partial_sum(begin(items), end(items)); }


inline auto lkp(item_cit first, item_cit last, weight_t capacity) -> float
{
    auto [split, total] = split_item(first, last, capacity);
    if (split == last) return total.profit;
    return total.profit + (capacity - total.weight) * split->efficiency();
}

auto lkp(Instance const& kp) { return lkp(begin(kp.items), end(kp.items), kp.capacity); }


auto lkp(item_vec const& Σ, item_id i, weight_t capacity) -> float
{
    auto const offset = begin(Σ) + i;
    auto const w = capacity + offset->weight;
    auto const lb = rs::lower_bound(next(offset), end(Σ), w, {}, &Item::weight);
    if (lb == end(Σ)) return lb->profit - offset->profit; // all items can fit
    auto const plb = prev(lb);
    return plb->profit + (w - plb->weight) * (*lb - *plb).efficiency() - offset->profit;
}

/* Returns a vector w such as w[i] is minimum weight among the items i to the last one. */
auto minimum_weight_vec(Instance const& kp) -> std::vector<weight_t>
{
    std::vector<weight_t> min_weights(kp.n);
    min_weights.back() = kp.items.back().weight;
    auto item = next(rbegin(kp.items));
    for (auto it = next(rbegin(min_weights)); it != rend(min_weights); ++it, ++item)
        *it = std::min(*prev(it), item->weight);
    return min_weights;
}

/* Returns false if the solution is well-formed, true otherwise. */
auto check_solution(Instance const& kp, AssignmentSolution const& solution) -> bool
{
    if (kp.n != size(solution.assignment)) {
        std::puts("[Error] Solution's size is incorrect.");
        return false;
    }

    auto total = utils::accumulate_if(begin(kp.items), end(kp.items),
        begin(solution.assignment), Item {0, 0});

    if (total.profit != solution.profit)
        std::puts("[Error] Solution's profit is incorrect.");
    if (total.weight != solution.weight)
        std::puts("[Error] Solution's weight is incorrect.");
    if (total.weight > kp.capacity)
        std::puts("[Error] The solution violates the capacity constraint.");

    return total.profit != solution.profit or total.weight != solution.weight or
        total.weight > kp.capacity;
}

} 