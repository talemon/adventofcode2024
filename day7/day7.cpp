#include <algorithm>
#include <charconv>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

struct Equation
{
    uint64_t m_TestValue;
    std::vector<uint32_t> m_Operands;
};

static bool CheckBackwards(std::span<const uint32_t> operands, uint64_t testValue)
{
    if (operands.size() == 1)
        return operands.front() == testValue;
    
    const auto last = operands.back();
    if (testValue % last == 0)
    {
        if (CheckBackwards(operands.subspan(0, operands.size() - 1), testValue / last))
        {
            return true;
        }
    }
    return CheckBackwards(operands.subspan(0, operands.size() - 1), testValue - last);
}

static bool CheckForwards(std::span<const uint32_t> operands, uint64_t acc, uint64_t target)
{
    if (operands.empty())
    {
        return target == acc;
    }
    const auto first = operands.front();

    if (CheckForwards(operands.subspan(1), acc + first, target))
    {
        return true;
    }
    if (acc == 0)
        return false;
    
    if (CheckForwards(operands.subspan(1), acc * first, target))
    {
        return true;
    }

    auto digitCount = static_cast<uint32_t>(std::log10(first) + 1);
    
    acc = acc * (std::pow(10, digitCount)) + first;
    return CheckForwards(operands.subspan(1), acc, target);
}

static Equation ParseLine(std::string_view line)
{
    Equation equation;

    auto colon = line.find(':');
    if (colon == std::string::npos)
        throw std::invalid_argument("Invalid input string");

    auto testValueView = line.substr(0, colon);

    std::from_chars(testValueView.data(), testValueView.data() + testValueView.size(), equation.m_TestValue);

    using std::operator ""sv;
    constexpr auto delimiter{" "sv};

    auto operandsView = line.substr(colon + 2);

    for (auto value : std::views::split(operandsView, delimiter))
    {
        equation.m_Operands.push_back(std::stoul(std::string(value.data(), value.size())));
    }

    return equation;
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

    std::vector<Equation> equations;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        equations.emplace_back(ParseLine(line));
    }

    uint64_t sum = std::accumulate(equations.begin(), equations.end(), 0ui64, [&](uint64_t acc, const Equation& eq)
    {
        if (CheckForwards(std::span(eq.m_Operands), 0, eq.m_TestValue))
        // if (CheckBackwards(std::span(eq.m_Operands), eq.m_TestValue))
        {
            return acc + eq.m_TestValue;
        }
        return acc;
    });

    std::cout << "Sum: " << sum << '\n';

    return 0;
}
