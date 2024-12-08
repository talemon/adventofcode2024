#include <algorithm>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

std::vector<int> ParseOrder(std::string_view input)
{
    using std::operator ""sv;
    constexpr auto delimiter{","sv};

    std::vector<int> result;

    for (auto value : std::views::split(input, delimiter))
    {
        result.push_back(std::stoi(std::string(value.data(), value.size())));
    }

    return result;
}

std::pair<int, int> ParseRulePair(const std::string& str)
{
    auto separator = str.find('|');
    if (separator == std::string::npos)
        throw std::invalid_argument("invalid syntax");

    auto id1 = str.substr(0, separator);
    auto id2 = str.substr(separator + 1);

    return std::make_pair(std::stoi(id1), std::stoi(id2));
}

struct PageComparer
{
    std::reference_wrapper<const std::vector<std::pair<int, int>>> m_rules;

    bool operator()(int lhs, int rhs) const
    {
        auto leftSmaller = std::ranges::any_of(m_rules.get(), [pair = std::make_pair(lhs, rhs)](const auto& rule)
        {
            return rule == pair;
        });

        return leftSmaller;
    }
};

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

    std::vector<std::pair<int, int>> rules;
    std::vector<std::vector<int>> orders;

    std::string line;
    while (std::getline(file, line) && !line.empty())
    {
        rules.push_back(ParseRulePair(line));
    }

    while (std::getline(file, line))
    {
        orders.push_back(ParseOrder(line));
    }

    PageComparer comparer{rules};

    auto printOrder = [](const std::vector<int>& order)
    {
        std::cout << "Order: ";
        std::ranges::for_each(order, [](int page)
        {
            std::cout << page << " ";
        });
    };

    auto getMiddle = [](const std::vector<int>& order)
    {
        return order[order.size() / 2];
    };

    int total = 0;
    int corrected_total = 0;

    for (auto order : orders)
    {
        if (std::ranges::is_sorted(order, comparer))
        {
            printOrder(order);
            int middle = getMiddle(order);
            std::cout << "-  middle: " << middle << '\n';

            total += middle;
        }
        else
        {
            std::ranges::sort(order, comparer);
            corrected_total += getMiddle(order);
        }
    }

    std::cout << "Total: " << total << " - Corrected: " << corrected_total << '\n';

    return 0;
}
