#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>

#include "Matrix.h"

Matrix RemoveUnreachables(Matrix map, Vector2 startingPoint)
{
    auto otherTrailheads = map.FindAll('0');
    std::erase(otherTrailheads, startingPoint);

    std::ranges::for_each(otherTrailheads, [&](auto& pos)
    {
        map.Set(pos, '.');
    });

    auto isReachable = [&](Vector2& pos)
    {
        char neededValue = map.Get(pos) - 1;

        if (neededValue < '0')
            return true;

        auto up = pos + Vector2{0, -1};
        auto down = pos + Vector2{0, 1};
        auto left = pos + Vector2{-1, 0};
        auto right = pos + Vector2{1, 0};

        if (map.IsInside(up) && map.Get(up) == neededValue)
            return true;
        if (map.IsInside(down) && map.Get(down) == neededValue)
            return true;
        if (map.IsInside(left) && map.Get(left) == neededValue)
            return true;
        if (map.IsInside(right) && map.Get(right) == neededValue)
            return true;
        return false;
    };

    for (char height = '1'; height <= '9'; ++height)
    {
        auto nodes = map.FindAll(height);
        std::ranges::for_each(nodes, [&](auto& pos)
        {
            if (!isReachable(pos))
                map.Set(pos, '.');
        });
    }

    return map;
}

size_t CountTrails(const Matrix& map, Vector2 startingPoint)
{
    return RemoveUnreachables(map, startingPoint).Count('9');
}

void SolvePart1(const std::vector<Vector2>& trailheads, const Matrix& map)
{
    int score = 0;

    std::ranges::for_each(trailheads, [&](auto& pos)
    {
        score += CountTrails(map, pos);
    });

    std::cout << "Sum: " << score << '\n';
}

int CountPath(const Matrix& map, Vector2 pos)
{
    const char nodeValue = map.Get(pos);

    auto canWalkTo = [&](char target)
    {
        if (target < '0' || target > '9')
            return false;

        return target - nodeValue == 1;
    };

    if (nodeValue == '9')
        return 1;

    if (nodeValue < '0' || nodeValue > '9')
        return 0;

    auto up = pos + Vector2{0, -1};
    auto down = pos + Vector2{0, 1};
    auto left = pos + Vector2{-1, 0};
    auto right = pos + Vector2{1, 0};

    int count = 0;

    if (map.IsInside(up) && canWalkTo(map.Get(up)))
        count += CountPath(map, up);
    if (map.IsInside(down) && canWalkTo(map.Get(down)))
        count += CountPath(map, down);
    if (map.IsInside(left) && canWalkTo(map.Get(left)))
        count += CountPath(map, left);
    if (map.IsInside(right) && canWalkTo(map.Get(right)))
        count += CountPath(map, right);

    return count;
}

int CalculateTrailScore(const Matrix& map, Vector2 startingPoint)
{
    auto trailMap = RemoveUnreachables(map, startingPoint);
    return CountPath(trailMap, startingPoint);
}

void SolvePart2(const std::vector<Vector2>& trailheads, const Matrix& map)
{
    int rating = 0;

    std::ranges::for_each(trailheads, [&](auto& pos)
    {
        rating += CalculateTrailScore(map, pos);
    });

    std::cout << "Rating: " << rating << '\n';
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

    std::vector<Vector2> trailheads = map.FindAll('0');

    // SolvePart1(trailheads, map);
    SolvePart2(trailheads, map);

    return 0;
}
