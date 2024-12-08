#pragma once
#include "Matrix.h"

class GameMap
{
public:
    enum class Direction : uint8_t
    {
        Invalid,
        North,
        East,
        South,
        West
    };

    bool IsValidPosition(Vector2 pos) const;

    enum class StepResult : uint8_t
    {
        HitWall,
        WentOut,
        Looping,
        Done
    };

    explicit GameMap(Matrix data);

    Vector2 GetSize() const { return m_Data.GetSize(); }

    void Print() const { m_Data.Print(); }
    int CountMoved() const;

    StepResult Step();
    StepResult StepUntilEnd();

    char Get(Vector2 pos) const { return m_Data.Get(pos); }
    void PutObstacle(Vector2 pos) { m_Data.Set(pos, '#'); }
    
private:
    Matrix m_Data;
    Vector2 m_Pos;
    Direction m_Direction = Direction::Invalid;
};
