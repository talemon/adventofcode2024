#include "Matrix.h"

#include <algorithm>
#include <cassert>
#include <iostream>

void Matrix::Print() const
{
    std::string_view view { m_Data.data(), m_DataSize};

    for (int offset = 0; offset < m_DataSize; offset += m_Stride)
    {
        std::cout << view.substr(offset, m_Stride) << '\n';
    }
}

Vector2 Matrix::GetSize() const
{
    return Vector2(m_Stride, m_DataSize / m_Stride);
}

char Matrix::Get(uint32_t x, uint32_t y) const
{
    const size_t offset = y * m_Stride + x;
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

void Matrix::Set(uint32_t x, uint32_t y, char value)
{
    const size_t offset = y * m_Stride + x;
    assert(offset < m_DataSize);

    m_Data[offset] = value;
}

Vector2 Matrix::Find(char ch) const
{
    auto itr = std::ranges::find(m_Data, ch);
    auto offset = std::distance(m_Data.begin(), itr);
    return GetPosition(offset);
}

void Matrix::Transpose()
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

uint32_t Matrix::GetOffset(uint32_t x, uint32_t y) const
{
    return y * m_Stride + x;
}

Vector2 Matrix::GetPosition(uint32_t offset) const
{
    int y = offset / m_Stride;
    int x = offset % m_Stride;
    return {x, y};
}
