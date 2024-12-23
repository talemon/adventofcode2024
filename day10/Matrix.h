﻿#pragma once
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

    auto operator<=>(const Vector2&) const = default;

    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
    {
        return {lhs.m_X + rhs.m_X, lhs.m_Y + rhs.m_Y};
    }

    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
    {
        return {lhs.m_X - rhs.m_X, lhs.m_Y - rhs.m_Y};
    }

    friend Vector2 operator*(const Vector2& lhs, int rhs)
    {
        return {lhs.m_X * rhs, lhs.m_Y * rhs};
    }

    int m_X;
    int m_Y;
};

struct Matrix
{
    Matrix(const std::vector<char>& data, size_t stride)
        : m_Data(data),
          m_Width(stride)
    {
        m_DataSize = data.size();
        assert(data.size() % m_Width == 0);
        m_Height = m_DataSize / m_Width;
    }

    Matrix(const std::vector<char>& data)
        : m_Data(data)
    {
        m_DataSize = data.size();
        m_Width = std::sqrt(data.size());
        m_Height = m_Width;
    }

    explicit Matrix(const Vector2& size)
        : m_Width(size.m_X), m_Height(size.m_Y)
    {
        m_Data.resize(m_Width * m_Height);
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

    void Reset(char ch);

    bool IsInside(Vector2 coord) const;

    Vector2 Find(char ch) const;
    std::vector<Vector2> FindAll(char ch) const;

    void Transpose();

    size_t GetOffset(uint32_t x, uint32_t y) const;
    Vector2 GetPosition(size_t offset) const;

    size_t Width() const { return m_Width; }
    size_t Height() const { return m_Height; }

    size_t Count(char ch) const;

private:
    std::vector<char> m_Data;
    size_t m_Width;
    size_t m_Height;
    size_t m_DataSize;
};
