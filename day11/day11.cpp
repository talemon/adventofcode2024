#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <map>
#include <vector>

// thanks to Vitali at https://stackoverflow.com/a/1489873
constexpr int numDigits(int64_t x)
{
    if (x == std::numeric_limits<int64_t>::max())
        return 19 + 1;

    if (x >= 10000000000) {
        if (x >= 100000000000000) {
            if (x >= 10000000000000000) {
                if (x >= 100000000000000000) {
                    if (x >= 1000000000000000000)
                        return 19;
                    return 18;
                }
                return 17;
            }
            if (x >= 1000000000000000)
                return 16;
            return 15;
        }
        if (x >= 1000000000000) {
            if (x >= 10000000000000)
                return 14;
            return 13;
        }
        if (x >= 100000000000)
            return 12;
        return 11;
    }
    if (x >= 100000) {
        if (x >= 10000000) {
            if (x >= 100000000) {
                if (x >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (x >= 1000000)
            return 7;
        return 6;
    }
    if (x >= 100) {
        if (x >= 1000) {
            if (x >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if (x >= 10)
        return 2;
    return 1;
}

static constexpr std::array<int64_t, 20> pow10 = {
        1, 10, 100, 1000, 10000,
        100000, 1000000, 10000000, 100000000, 1000000000,
        10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000,
        100000000000000, 1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000
};

uint64_t BlinkAndCountCached(int64_t number, size_t count, std::map<std::pair<int64_t, size_t>, uint64_t>& cache)
{
    auto test = std::numeric_limits<uint64_t>::max();

    if (count == 0)
        return 1;

    std::pair<int64_t, size_t> key = { number, count };
    if (cache.contains(key))
    {
        return cache[key];
    }

    uint64_t result = 0;
    if (number == 0)
    {
        result = BlinkAndCountCached(1, count - 1, cache);
    }
    else
    {
        int digitCount = numDigits(number);

        if (digitCount % 2 == 0)
        {
            digitCount = digitCount / 2;
            const int64_t decimal_divisor = pow10[digitCount];

            const auto division = std::div(number, decimal_divisor);
            result = BlinkAndCountCached(division.quot, count - 1, cache) + BlinkAndCountCached(division.rem, count - 1, cache);
        }
        else
        {
            result = BlinkAndCountCached(number * 2024, count - 1, cache);
        }
    }
    
    cache[key] = result;
    return result;
}

int main(int argc, char* argv[])
{
    constexpr size_t BLINK_COUNT = 75;
    auto printStones = [](const std::vector<int64_t>& stones)
        {
            std::cout << "Stones: ";
            std::ranges::copy(stones, std::ostream_iterator<int>(std::cout, " "));
            std::cout << '\n';
        };

    std::map<std::pair<int64_t, size_t>, uint64_t> cache;

    //std::vector<int64_t> input = {125, 17};
    std::vector<uint64_t> input = { 890, 0, 1, 935698, 68001, 3441397, 7221, 27 };

    // this actually overflows, need to find a better representation for large numbers
    uint64_t sum = std::accumulate(input.begin(), input.end(), 0, [&](uint64_t acc, int64_t number)
        {
            uint64_t count = BlinkAndCountCached(number, BLINK_COUNT, cache);
            std::cout << "result for " << number << " is " << count << "\n";
            return acc + count;
        });

    std::cout << "Count: " << sum << '\n';

    return 0;
}
