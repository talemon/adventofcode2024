#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>

#include "Matrix.h"

void CalculateAntiNodes(const std::vector<char>& antennas, const Matrix& sourceMap)
{
    Matrix targetMap {sourceMap.GetSize()};
    targetMap.Reset('.');
    
    auto getAntiNode = [](Vector2 a, Vector2 b)
    {
        return (a * 2) - b;
    };
    
    std::ranges::for_each(antennas, [&](char antennaId)
    {
        auto towerPositions = sourceMap.FindAll(antennaId);
        size_t towerCount = towerPositions.size();
        
        for (int i = 0; i < towerCount; ++i)
        {
            for (int j = i + 1; j < towerCount; ++j)
            {
                auto nodePos = getAntiNode(towerPositions[i], towerPositions[j]);
                if (targetMap.IsInside(nodePos))
                    targetMap.Set(nodePos, '#');
                nodePos = getAntiNode(towerPositions[j], towerPositions[i]);
                if (targetMap.IsInside(nodePos))
                    targetMap.Set(nodePos, '#');
            }
        }
    });

    std::cout << "Count: " << targetMap.Count('#') << '\n';
}

void CalculateResonantNodes(const std::vector<char>& antennas, const Matrix& sourceMap)
{
    Matrix targetMap {sourceMap.GetSize()};
    targetMap.Reset('.');
    
    auto getResonantNode = [](Vector2 a, Vector2 b, int index)
    {
        auto diff = a - b;
        return a + (diff * index);
    };
    
    std::ranges::for_each(antennas, [&](char antennaId)
    {
        auto towerPositions = sourceMap.FindAll(antennaId);
        size_t towerCount = towerPositions.size();
        
        for (int i = 0; i < towerCount; ++i)
        {
            for (int j = i + 1; j < towerCount; ++j)
            {
                auto tower1 = towerPositions[i];
                auto tower2 = towerPositions[j];

                int index = 0;
                auto nodePos = getResonantNode(tower1, tower2, index);
                while (targetMap.IsInside(nodePos))
                {
                    targetMap.Set(nodePos, '#');
                    nodePos = getResonantNode(tower1, tower2, ++index);
                }

                index = 0;
                nodePos = getResonantNode(tower2, tower1, index);
                while (targetMap.IsInside(nodePos))
                {
                    targetMap.Set(nodePos, '#');
                    nodePos = getResonantNode(tower2, tower1, ++index);
                }
            }
        }
    });

    std::cout << "Count: " << targetMap.Count('#') << '\n';
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>" << '\n';
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open())
    {
        std::cerr << "Unable to open file " << argv[1] << '\n';
        return 1;
    }

    std::istream_iterator<char> streamIt{file};

    auto map = Matrix{std::vector<char>{streamIt, std::istream_iterator<char>()}};
    
    auto antennas = map.Data();
    erase_if(antennas, std::not_fn(std::isalnum));

    std::ranges::sort(antennas);
    auto unique_range = std::ranges::unique(antennas);
    antennas.erase(unique_range.begin(), unique_range.end());

    CalculateResonantNodes(antennas, map);

    return 0;
}
