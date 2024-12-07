#pragma once
#include <cassert>
#include <span>
#include <string_view>
#include <vector>

struct SquareMatrix
{
    SquareMatrix(const std::vector<char>& data, uint32_t stride)
        : m_Data(data),
          m_Stride(stride)
    {
        assert(data.size() == (m_Stride * m_Stride));
    }

    explicit SquareMatrix(uint32_t stride)
        : m_Stride(stride)
    {
        m_Data.resize(m_Stride * m_Stride);
    }

    const std::vector<char>& Data() const { return m_Data; }
    std::vector<char> ExtractDiagonal1() const;
    std::vector<char> ExtractDiagonal2() const;
    SquareMatrix ExtractWindow(uint32_t x, uint32_t y, uint32_t size) const;

    char Get(uint32_t x, uint32_t y) const;
    std::vector<char>::const_iterator GetIterator(uint32_t x, uint32_t y) const;
    std::vector<char>::iterator GetIterator(uint32_t x, uint32_t y);

    int CountRows(std::string_view search_word) const;
    int CountDiagonals(std::string_view search_word) const;
    bool CheckDiagonals(std::string_view search_word) const;

    void Transpose();

private:
    uint32_t GetOffset(uint32_t x, uint32_t y) const;
    static int CountRange(std::span<const char> range, std::string_view search_word, std::string_view word_reverse);

    std::vector<char> m_Data;
    uint32_t m_Stride;
};
