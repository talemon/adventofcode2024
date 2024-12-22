#include "Matrix.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>

void Matrix::Print() const
{
    std::string_view view { m_Data.data(), m_DataSize};

    for (int offset = 0; offset < m_DataSize; offset += m_Width)
    {
        std::cout << view.substr(offset, m_Width) << '\n';
    }
}

Vector2 Matrix::GetSize() const
{
    return Vector2(m_Width, m_DataSize / m_Width);
}

char Matrix::Get(uint32_t x, uint32_t y) const
{
    const size_t offset = y * m_Width + x;
    assert(offset < m_DataSize);

    return m_Data[offset];
}

char Matrix::Get(Vector2 coord) const
{
    return Get(coord.m_X, coord.m_Y);
}

void Matrix::Set(Vector2 coord, char value)
{
    Set(coord.m_X, coord.m_Y, value);
}

void Matrix::Reset(char ch)
{
    std::ranges::fill(m_Data, ch);
}

bool Matrix::IsInside(Vector2 coord) const
{
    return coord.m_X >= 0 && coord.m_Y >= 0 && coord.m_X < m_Width && coord.m_Y < m_Height;
}

void Matrix::Set(uint32_t x, uint32_t y, char value)
{
    const size_t offset = y * m_Width + x;
    assert(offset < m_DataSize);

    m_Data[offset] = value;
}

Vector2 Matrix::Find(char ch) const
{
    auto itr = std::ranges::find(m_Data, ch);
    auto offset = std::distance(m_Data.begin(), itr);
    return GetPosition(offset);
}

std::vector<Vector2> Matrix::FindAll(char ch) const
{
    std::vector<Vector2> result;
    
    for (size_t offset = 0; offset < m_DataSize; ++offset)
    {
        if (m_Data[offset] == ch)
        {
            result.emplace_back(GetPosition(offset));
        }
    }
    return result;
}

void Matrix::Transpose()
{
    for (int row = 0; row < m_Width; ++row)
    {
        for (int col = row + 1; col < m_Width; ++col)
        {
            auto pos1 = GetOffset(row, col);
            auto pos2 = GetOffset(col, row);

            std::swap(m_Data[pos1], m_Data[pos2]);
        }
    }
}

size_t Matrix::GetOffset(uint32_t x, uint32_t y) const
{
    return y * m_Width + x;
}

Vector2 Matrix::GetPosition(size_t offset) const
{
    int y = offset / m_Width;
    int x = offset % m_Width;
    return {x, y};
}

size_t Matrix::Count(char ch) const
{
    return std::ranges::count(m_Data, ch);
}
