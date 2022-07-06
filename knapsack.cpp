
#include "knapsack.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

Item::Item(int inWeight, int inValue)
: Weight { inWeight },
  Value { inValue}
{ }

Knapsack::Knapsack()
: m_MaxKnapsackWeight {0}
{ }

void Knapsack::ReadInputFile(const string& sInputFile)
{
    std::ifstream ips(sInputFile);

    if(! ips.is_open())
        return;

    string sLine;

    getline(ips, sLine);

    std::stringstream sStream(sLine);

    int NumItems;

    sStream >> m_MaxKnapsackWeight >> NumItems;

    m_Items.reserve(NumItems);

    while(getline(ips, sLine))
    {
        auto Value {0}, Weight {0};
        std::stringstream strm(sLine);
        strm >> Value >> Weight;

        m_Items.push_back( Item {Weight, Value} );
    }

//    std::cout << m_Items.size() << "\n";
//    std::cout << m_Items[m_Items.size()-1].Value << " " << m_Items[m_Items.size()-1].Weight << "\n";

    return;
}

int Knapsack::CalcKnapsackValue()
{
    int CompArray[m_Items.size()][m_MaxKnapsackWeight] = {};

    for(int init = m_Items[0].Weight - 1; init < m_MaxKnapsackWeight; init++)
        CompArray[0][init] = m_Items[0].Value;

    for(int i = 1; i < m_Items.size(); i++)
    {
        for(int x = 0; x < m_MaxKnapsackWeight; x++)
        {
            auto ItemChosen {0}, ItemNotChosen {0};

            ItemNotChosen = CompArray[i-1][x];

            if(x - m_Items[i].Weight >= 0)
                ItemChosen = CompArray[i-1][x - m_Items[i].Weight] + m_Items[i].Value;

            CompArray[i][x] = std::max(ItemChosen, ItemNotChosen);
            //std::cout << "i " << i << " x " << x << " chosen " << ItemChosen << " not chosen " << ItemNotChosen << "\n";
        }
    }

    return CompArray[m_Items.size()-1][m_MaxKnapsackWeight-1];
}

int Knapsack::CalcKnapsackValueRecurse()
{
    return OptimalKnapsack(m_MaxKnapsackWeight, m_Items.size());
}

int Knapsack::OptimalKnapsack(int nWeight, int nSize)
{
    // base case
    if(nWeight == 0 || nSize == 0)
        return 0;

    // check if this is previously calculated, if yes, return it
    auto MapIt = m_NWMap.find(NumWeight{nSize, nWeight});
    if(MapIt != m_NWMap.end())
        return MapIt->second;

    auto solution {0};

    // if the item will not fit in knapsack, recurse to next item
    if(m_Items[nSize-1].Weight > nWeight)
        solution = OptimalKnapsack(nWeight, nSize - 1);
    else
        solution = std::max( OptimalKnapsack(nWeight, nSize -1), // not chosen
                     OptimalKnapsack(nWeight - m_Items[nSize-1].Weight, nSize - 1) + m_Items[nSize-1].Value  // chosen
                    );

    // store the computed solution for later use
    m_NWMap[NumWeight{nSize, nWeight}] = solution;
    return solution;
}

int main()
{
    Knapsack ks;
    ks.ReadInputFile("input.txt");
    std::cout << ks.CalcKnapsackValue();
//    std::cout << ks.CalcKnapsackValueRecurse();

    return 0;
}

