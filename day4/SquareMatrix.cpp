#include "SquareMatrix.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iterator>

std::vector<char> SquareMatrix::ExtractDiagonal1() const
{
    std::vector<char> result;
    result.reserve(m_Stride);

    for (uint32_t i = 0; i < m_Stride; ++i)
    {
        result.push_back(Get(i, i));
    }

    return result;
}

std::vector<char> SquareMatrix::ExtractDiagonal2() const
{
    std::vector<char> result;
    result.reserve(m_Stride);

    for (uint32_t i = 0; i < m_Stride; ++i)
    {
        result.push_back(Get(m_Stride - i - 1, i));
    }

    return result;
}

SquareMatrix SquareMatrix::ExtractWindow(uint32_t x, uint32_t y, uint32_t size) const
{
    assert(x < m_Stride && y < m_Stride);
    assert(x + size <= m_Stride && y + size <= m_Stride);

    SquareMatrix output {size};
    output.m_Data.clear();

    for (uint32_t row = 0; row < size; ++row)
    {
        auto itr = GetIterator(x, y + row);
        
        std::copy_n(itr, size, std::back_inserter(output.m_Data));
    }

    return output;
}

char SquareMatrix::Get(uint32_t x, uint32_t y) const
{
    const size_t offset = y * m_Stride + x;
    assert(offset < m_Data.max_size());

    return m_Data[offset];
}

std::vector<char>::const_iterator SquareMatrix::GetIterator(uint32_t x, uint32_t y) const
{
    return m_Data.begin() + GetOffset(x, y);
}

std::vector<char>::iterator SquareMatrix::GetIterator(uint32_t x, uint32_t y)
{
    return m_Data.begin() + GetOffset(x, y);
}

int SquareMatrix::CountRows(std::string_view search_word) const
{
    std::string reverse_word{search_word};
    std::ranges::reverse(reverse_word);

    int count = 0;
    for (uint32_t row = 0; row < m_Stride; ++row)
    {
        auto rowRange = std::span(m_Data.begin() + GetOffset(0, row), m_Stride);
        count += CountRange(rowRange, search_word, reverse_word);
    }
    return count;
}

int SquareMatrix::CountDiagonals(std::string_view search_word) const
{
    auto diagonal1 = ExtractDiagonal1();
    auto diagonal2 = ExtractDiagonal2();

    std::string reverse_word{search_word};
    std::ranges::reverse(reverse_word);
    
    return CountRange(diagonal1, search_word, reverse_word) + CountRange(diagonal2, search_word, reverse_word);
}

bool SquareMatrix::CheckDiagonals(std::string_view search_word) const
{
    auto diagonal1 = ExtractDiagonal1();
    auto diagonal2 = ExtractDiagonal2();

    std::string reverse_word{search_word};
    std::ranges::reverse(reverse_word);
    
    return CountRange(diagonal1, search_word, reverse_word) && CountRange(diagonal2, search_word, reverse_word);
}

void SquareMatrix::Transpose()
{
    for (int row = 0; row < m_Stride; ++row)
    {
        for (int col = row + 1; col < m_Stride; ++col)
        {
            auto pos1 = GetOffset(row, col);
            auto pos2 = GetOffset(col, row);

            std::swap(m_Data[pos1], m_Data[pos2]);
        }
    }
}

uint32_t SquareMatrix::GetOffset(uint32_t x, uint32_t y) const
{
    return y * m_Stride + x;
}

int SquareMatrix::CountRange(std::span<const char> range, std::string_view search_word, std::string_view word_reverse)
{
    auto search_len = search_word.length();

    int count = 0;
    for (std::size_t offset = 0; offset <= range.size() - search_len; ++offset)
    {
        const char* ptr = range.data() + offset;
        if (memcmp(ptr, search_word.data(), search_len) == 0 ||
            memcmp(ptr, word_reverse.data(), search_len) == 0)
        {
            ++count;
        }
    }

    return count;
}
