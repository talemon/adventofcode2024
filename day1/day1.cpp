#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

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
    }

    std::vector<int> list1;
    std::vector<int> list2;
    
    while (!file.eof())
    {
        int first, second;
        file >> first >> second;
        list1.push_back(first);
        list2.push_back(second);
    }

    std::ranges::sort(list1);
    std::ranges::sort(list2);

    auto total_distance = std::transform_reduce(
        list1.begin(),
        list1.end(),
        list2.begin(),
        0,
        std::plus<>(),
        [](int first, int second)
        {
            return first < second ? second - first : first - second;
        });

    auto similarityScore = [&list2](int id)
    {
        auto [begin, end] = std::ranges::equal_range(list2, id);
        return id * std::distance(begin, end);
    };

    auto similarity = std::transform_reduce(list1.begin(), list1.end(), 0ll, std::plus(), similarityScore);

    std::cout << "Distance: " << total_distance << " Similarity: " << similarity << '\n';

    return 0;
}
