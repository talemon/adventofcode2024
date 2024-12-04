#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    uint8_t get_sign(int value)
    {
        return (0 < value) - (value < 0);
    }
    
    template <std::ranges::input_range Range>
    auto find_error(const Range& report)
    {
        int last_number = report.front();
        uint8_t sign = 0;
        
        auto errorItr = std::find_if_not(report.begin() + 1, report.end(), [&](int number)
        {
            const int diff = number - last_number;
            const int distance = std::abs(diff);
            if (distance < 1 || distance > 3)
                return false;

            const auto new_sign = get_sign(diff);
            if (sign == 0)
                sign = new_sign;
            else if (sign != new_sign)
                return false;
        
            last_number = number;
            return true;
        });
        
        return errorItr;
    }

    std::vector<int> parse_line(const std::string& line)
    {
        std::istringstream stream(line);
        std::vector<int> numbers;

        while (stream.good())
        {
            int number = 0;
            stream >> number;
            numbers.push_back(number);
        }

        return numbers;
    }

    std::vector<std::vector<int>> parse_file(const std::string& filename)
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "Unable to open file " << filename << '\n';
        }

        std::vector<std::vector<int>> result;
        std::string line;
        while (std::getline(file, line))
        {
            result.emplace_back(parse_line(line));
        }
        return result;
    }

    bool check_data(const std::vector<int>& report)
    {
        return find_error(report) == report.end();
    }

    bool check_data_tolerant(const std::vector<int>& report)
    {
        auto report_copy = report;
        auto forward_error = find_error(report_copy);
        if (forward_error != report_copy.end())
        {
            report_copy.erase(forward_error);
            if (find_error(report_copy) == report_copy.end())
                return true;

            report_copy = report;
            std::ranges::reverse(report_copy);
            auto backward_error = find_error(report_copy);
            report_copy.erase(backward_error);
            return find_error(report_copy) == report_copy.end();
        }
        return true;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>" << '\n';
        return 1;
    }

    auto data = parse_file(argv[1]);
    auto safe_reports = std::ranges::count_if(data, check_data);
    
    std::cout << "Safe entries: " << safe_reports << '\n';

    auto kinda_safe_reports = std::ranges::count_if(data, check_data_tolerant);
    
    std::cout << "Kinda safe entries: " << kinda_safe_reports << '\n';
    
    return 0;
}
