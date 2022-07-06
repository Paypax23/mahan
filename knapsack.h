
#include <vector>
#include <string>
#include "mymap.h"

using std::string;
using std::vector;

class Item
{
public:
    Item(int inWeight = 0, int inValue = 0);

    int Weight;
    int Value;
};

class Knapsack
{
public:
    Knapsack();
    void ReadInputFile(const string& sInputFile);
    int CalcKnapsackValue();
    int CalcKnapsackValueRecurse();

private:
    vector<Item> m_Items;
    int m_MaxKnapsackWeight;
    unordered_map<NumWeight, int, NumWeightHash, NumWeightEqual> m_NWMap;

    int OptimalKnapsack(int nWeight, int nSize);
};