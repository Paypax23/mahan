#include<unordered_map>

using std::unordered_map;

class NumWeight
{
public:
    NumWeight(int inNum=0, int inWeight=0) : Num {inNum}, Weight {inWeight} {}

    int Num;
    int Weight;
};

class NumWeightHash
{
public:
    size_t operator()(const NumWeight& nw) const
    {
        return std::hash<int>()(nw.Num) ^ std::hash<int>()(nw.Weight);
    }
};

class NumWeightEqual
{
public:
    bool operator()(const NumWeight& lhs, const NumWeight& rhs) const
    {
        return lhs.Num == rhs.Num && lhs.Weight == rhs.Weight;
    }
};
