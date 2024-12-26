#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

#include "Matrix.h"

std::set<Vector2> GetRegion(const Matrix& map, const Vector2& start)
{
    const char plant = map.Get(start);
    std::set<Vector2> region = {start};
    std::vector<Vector2> to_search = {start};

    while (!to_search.empty())
    {
        Vector2 pos = to_search.back();
        to_search.pop_back();

        if (Vector2 north = pos.North();
            map.IsInside(north) && map.Get(north) == plant && !region.contains(north))
        {
            region.insert(north);
            to_search.push_back(north);
        }
        if (Vector2 south = pos.South();
            map.IsInside(south) && map.Get(south) == plant && !region.contains(south))
        {
            region.insert(south);
            to_search.push_back(south);
        }
        if (Vector2 east = pos.East();
            map.IsInside(east) && map.Get(east) == plant && !region.contains(east))
        {
            region.insert(east);
            to_search.push_back(east);
        }
        if (Vector2 west = pos.West();
            map.IsInside(west) && map.Get(west) == plant && !region.contains(west))
        {
            region.insert(west);
            to_search.push_back(west);
        }
    }

    return region;
}

template<class T>
requires std::ranges::input_range<T> && std::ranges::sized_range<T>
int CalculateRegionCost1(T&& fields, const Matrix& map)
{
    int area = fields.size();
    if (area == 0)
        return 0;

    char region = map.Get(*fields.begin());

    int circumference = std::accumulate(
        fields.begin(),
        fields.end(),
        0,
        [&map, region](int acc, const Vector2& field)
        {
            auto neighbours = map.GetNeighbours(field);
            int sum = 0;

            if (neighbours.north != region)
                ++sum;
            if (neighbours.east != region)
                ++sum;
            if (neighbours.south != region)
                ++sum;
            if (neighbours.west != region)
                ++sum;

            return acc + sum;
        });

    return area * circumference;
}

template<class T>
requires std::ranges::input_range<T> && std::ranges::sized_range<T>
int CalculateRegionCost2(T&& region, const Matrix& map)
{
    int area = region.size();
    if (area == 0)
        return 0;

    char region_id = map.Get(*region.begin());

    Vector2 center = Vector2::One();
    std::vector<Vector2> neighbours = {center.North(), center.NorthEast(), center.East(), center.SouthEast(), center.South(), center.SouthWest(), center.West(), center.NorthWest()};
    
    int corner_count = 0;
    
    for (Vector2 pos : region)
    {
        Matrix sample = map.Sample(pos, 1);

        for (int cornerIdx = 1; cornerIdx < neighbours.size(); cornerIdx += 2)
        {
            const Vector2 corner = neighbours[cornerIdx];
            const Vector2 side1 = neighbours[cornerIdx - 1];
            const Vector2 side2 = neighbours[(cornerIdx + 1) % neighbours.size()];

            bool side1_full = sample[side1] == region_id;
            bool side2_full = sample[side2] == region_id;
            bool corner_full = sample[corner] == region_id;
            
            if (!side1_full && !side2_full)
                ++corner_count;

            if (!corner_full && side1_full && side2_full)
                ++corner_count;
        }
    }

    return area * corner_count;
}

void ProcessMap(Matrix map)
{
    int total = 0;
    for (Vector2 pos = map.FindIf(std::isalpha); pos != Matrix::invalid_position; pos = map.FindIf(std::isalpha))
    {
        auto region = GetRegion(map, pos);
        int region_cost = CalculateRegionCost2(region, map);
        std::cout << "Region cost: " << region_cost << '\n';
        
        std::ranges::for_each(region, [&](Vector2 field)
        {
            map.Set(field, '.');
        });
        
        total += region_cost;
    }

    std::cout << "Total: " << total << '\n';
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

    ProcessMap(map);

    return 0;
}
