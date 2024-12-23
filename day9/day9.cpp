#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

struct DataBlock
{
    uint32_t m_id = std::numeric_limits<uint32_t>::max();

    [[nodiscard]]
    bool IsEmpty() const { return m_id == std::numeric_limits<uint32_t>::max(); }

    [[nodiscard]]
    bool IsSet() const { return !IsEmpty(); }

    void Clear() { m_id = std::numeric_limits<uint32_t>::max(); }

    void SetId(uint32_t id) { m_id = id; }

    [[nodiscard]]
    uint32_t GetId() const { return m_id; }

    friend bool operator==(const DataBlock& lhs, const DataBlock& rhs)
    {
        return lhs.m_id == rhs.m_id;
    }

    friend bool operator!=(const DataBlock& lhs, const DataBlock& rhs)
    {
        return !(lhs == rhs);
    }
};

struct DataBlockMatcher
{
    DataBlock m_Block;

    bool operator()(const DataBlock& other) const
    {
        return other.GetId() == m_Block.GetId();
    }
};

void DefragBlocks(std::vector<DataBlock>& drive)
{
    assert(!drive.empty(), "Can't defrag empty drive");

    auto spaceItr = std::ranges::find_if(drive, &DataBlock::IsEmpty);
    auto dataItr = drive.end() - 1;
    while (!dataItr->IsSet() && dataItr != spaceItr)
        --dataItr;

    while (spaceItr != dataItr && spaceItr != drive.end())
    {
        std::swap(*dataItr, *spaceItr);
        spaceItr = std::find(spaceItr + 1, dataItr, DataBlock());
        while (dataItr != spaceItr && dataItr->IsEmpty())
            --dataItr;
    }
}

void DefragFiles(std::vector<DataBlock>& drive)
{
    auto findSpace = [&](size_t size)
    {
        return std::ranges::search_n(drive, size, DataBlock());
    };

    auto getFileRange = [&](uint32_t fileId)
    {
        DataBlockMatcher matcher{fileId};
        auto startItr = std::find_if(drive.begin(), drive.end(), matcher);
        auto endItr = std::find_if_not(startItr, drive.end(), matcher);
        return std::ranges::subrange(startItr, endItr);
    };

    auto reverse_drive = std::ranges::reverse_view(drive);
    auto lastFileBlock = std::ranges::find_if(reverse_drive, &DataBlock::IsSet);
    assert(lastFileBlock != reverse_drive.end());

    auto fileId = lastFileBlock->GetId();

    while (fileId > 0)
    {
        auto fileRange = getFileRange(fileId);
        auto spaceRange = std::ranges::search_n(std::ranges::subrange(drive.begin(), fileRange.begin()), fileRange.size(), DataBlock());

        if (!spaceRange.empty())
        {
            std::ranges::swap_ranges(spaceRange, fileRange);
        }

        --fileId;
    }
}

std::vector<DataBlock> UnpackData(const std::vector<int>& data)
{
    size_t total_size = std::reduce(data.begin(), data.end(), 0);
    std::vector<DataBlock> output;
    output.resize(total_size);

    uint32_t fileId = 0;
    auto outItr = output.begin();
    bool nextIsSpace = false;

    std::ranges::for_each(data, [&](int value)
    {
        if (nextIsSpace)
        {
            std::advance(outItr, value);
        }
        else
        {
            outItr = std::fill_n(outItr, value, DataBlock{fileId});
            ++fileId;
        }
        nextIsSpace = !nextIsSpace;
    });

    return output;
}

std::vector<char> ReadFile(const std::string& filename)
{
    std::vector<char> data;

    if (std::ifstream file{filename, std::ios::binary | std::ios::ate})
    {
        auto size = file.tellg();
        data.resize(size);
        file.seekg(0, std::ios::beg);
        file.read(data.data(), size);

        while (!std::isdigit(data.back()))
        {
            data.pop_back();
        }
    }
    else
    {
        std::cerr << "Unable to open file " << filename << '\n';
    }
    return data;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>" << '\n';
        return 1;
    }

    std::vector<char> data = ReadFile(argv[1]);
    std::vector<int> parsed_data(data.size());
    std::ranges::transform(data, parsed_data.begin(), [](char ch) { return ch - '0'; });

    auto drive = UnpackData(parsed_data);

    // DefragBlocks(drive);
    DefragFiles(drive);

    uint64_t sum = 0;

    for (size_t i = 0; i < drive.size(); ++i)
    {
        if (drive[i].IsSet())
            sum += drive[i].GetId() * i;
    }

    std::cout << "Sum: " << sum << '\n';
    return 0;
}
