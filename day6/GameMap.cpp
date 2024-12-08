#include <algorithm>
#include <cassert>
#include <ranges>

#include "GameMap.h"

#include <cctype>

namespace
{
    constexpr Vector2 GetMovement(GameMap::Direction direction)
    {
        switch (direction)
        {
        case GameMap::Direction::North:
            return {0, -1};
        case GameMap::Direction::East:
            return {1, 0};
        case GameMap::Direction::South:
            return {0, 1};
        case GameMap::Direction::West:
            return {-1, 0};
        case GameMap::Direction::Invalid:
            break;
        }
        
        assert(false);
        return {0, 0};
    }

    GameMap::Direction RotateDirection(GameMap::Direction direction)
    {
        switch (direction)
        {
        case GameMap::Direction::North:
            return GameMap::Direction::East;
        case GameMap::Direction::East:
            return GameMap::Direction::South;
        case GameMap::Direction::South:
            return GameMap::Direction::West;
        case GameMap::Direction::West:
            return GameMap::Direction::North;
        case GameMap::Direction::Invalid:
            break;
        }

        assert(false);
        return GameMap::Direction::Invalid;
    }

    bool IsWalkable(char ch)
    {
        return ch == '.' || isdigit(ch);
    }

    bool IsVisited(char ch)
    {
        return isdigit(ch);
    }

    char VisitTile(char currentState)
    {
        if (currentState == '.')
            return '1';
        else if (isdigit(currentState) && currentState < '9')
            return ++currentState;
        
        assert(false);
        return '?';
    }
}

bool GameMap::IsValidPosition(Vector2 pos) const
{
    return pos.m_X >= 0 &&
        pos.m_Y >= 0 &&
        pos.m_X < m_Data.Stride() &&
        pos.m_Y < m_Data.Stride();
}

GameMap::GameMap(Matrix data)
    : m_Data(std::move(data))
{
    m_Pos = m_Data.Find('^');

    m_Data.Set(m_Pos, '.');
    m_Direction = Direction::North;
}

int GameMap::CountMoved() const
{
    return std::ranges::count_if(m_Data.Data(), IsVisited);
}

GameMap::StepResult GameMap::Step()
{
    Vector2 move = GetMovement(m_Direction);

    while (true)
    {
        Vector2 newPos = m_Pos + move;

        if (!IsValidPosition(newPos))
        {
            return StepResult::WentOut;
        }

        char newTile = m_Data.Get(newPos);
            
        if (IsWalkable(newTile))
        {
            m_Pos = newPos;

            char newState = VisitTile(newTile);
            
            if (newState > '7')
            {
                return StepResult::Looping;
            }
            else
            {
                m_Data.Set(m_Pos, newState);
            }
        }
        else if (m_Data.Get(newPos) == '#')
        {
            m_Direction = RotateDirection(m_Direction);
            return StepResult::HitWall;
        }
        else
        {
            assert(false);
        }
    }

    return StepResult::Done;
}

GameMap::StepResult GameMap::StepUntilEnd()
{
    StepResult result;
    do
    {
        result = Step();
    }
    while (result == StepResult::HitWall);

    return result;
}
