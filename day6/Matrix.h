#pragma once
#include <cassert>
#include <cmath>
#include <string_view>
#include <vector>

struct Vector2
{
    Vector2() = default;

    Vector2(int x, int y)
        : m_X(x), m_Y(y)
    {
    }

    friend bool operator<(const Vector2& lhs, const Vector2& rhs)
    {
        if (lhs.m_X < rhs.m_X)
            return true;
        if (rhs.m_X < lhs.m_X)
            return false;
        return lhs.m_Y < rhs.m_Y;
    }

    friend bool operator<=(const Vector2& lhs, const Vector2& rhs)
    {
        return !(rhs < lhs);
    }

    friend bool operator>(const Vector2& lhs, const Vector2& rhs)
    {
        return rhs < lhs;
    }

    friend bool operator>=(const Vector2& lhs, const Vector2& rhs)
    {
        return !(lhs < rhs);
    }

    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
    {
        return {lhs.m_X + rhs.m_X, lhs.m_Y + rhs.m_Y};
    }

    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
    {
        return {lhs.m_X - rhs.m_X, lhs.m_Y - rhs.m_Y};
    }

    int m_X;
    int m_Y;
};



struct Matrix
{
    Matrix(const std::vector<char>& data, uint32_t stride)
        : m_Data(data),
          m_Stride(stride)
    {
        m_DataSize = data.size();
        assert(data.size() == (m_Stride * m_Stride));
    }

    Matrix(const std::vector<char>& data)
        : m_Data(data)
    {
        m_DataSize = data.size();
        m_Stride = std::sqrt(data.size());
    }

    explicit Matrix(uint32_t stride)
        : m_Stride(stride)
    {
        m_Data.resize(m_Stride * m_Stride);
        m_DataSize = m_Data.size();
    }

    void Print() const;

    [[nodiscard]]
    const std::vector<char>& Data() const { return m_Data; }

    [[nodiscard]]
    Vector2 GetSize() const;

    [[nodiscard]]
    char Get(uint32_t x, uint32_t y) const;
    [[nodiscard]]
    char Get(Vector2 coord) const;
    void Set(uint32_t x, uint32_t y, char value);
    void Set(Vector2 coord, char value);

    Vector2 Find(char ch) const;

    void Transpose();

    uint32_t GetOffset(uint32_t x, uint32_t y) const;
    Vector2 GetPosition(uint32_t offset) const;

    uint32_t Stride() const { return m_Stride; }

private:
    std::vector<char> m_Data;
    uint32_t m_Stride;
    uint32_t m_DataSize;
};
