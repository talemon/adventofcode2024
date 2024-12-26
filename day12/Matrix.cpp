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

Vector2 Matrix::FindIf(const std::function<bool(char)>& predicate) const
{
    auto itr = std::ranges::find_if(m_Data, predicate);
    if (itr != m_Data.end())
    {
        return GetPosition(std::distance(m_Data.begin(), itr));
    }
    return {-1, -1};
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

Neighbours Matrix::GetNeighbours(Vector2 pos) const
{
    const Vector2 north = pos.North();
    const Vector2 south = pos.South();
    const Vector2 west = pos.West();
    const Vector2 east = pos.East();

    Neighbours output;
    if (IsInside(north))
        output.north = Get(north);
    if (IsInside(south))
        output.south = Get(south);
    if (IsInside(west))
        output.west = Get(west);
    if (IsInside(east))
        output.east = Get(east);

    return output;
}

Matrix Matrix::Sample(Vector2 center, int extent) const
{
    const int size = 1 + extent * 2;
    Matrix output {Vector2(size, size)};

    Vector2 start = center - Vector2(extent, extent);

    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            if (Vector2 pos = start + Vector2(x, y); IsInside(pos))
                output.Set(x, y, Get(pos));
            else
                output.Set(x, y, '.');
        }
    }

    return output;
}
