#include <fstream>
#include <iostream>

#include "GameMap.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>" << '\n';
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open())
    {
        std::cerr << "Unable to open file " << argv[1] << '\n';
        return 1;
    }

    std::istream_iterator<char> streamIt{file};

    GameMap map{Matrix{std::vector<char>{streamIt, std::istream_iterator<char>()}}};

    auto size = map.GetSize();
    
    if (argc == 3)
    {
        map.PutObstacle({0,0});
        
        int repeat = std::atoi(argv[2]);
        for (int i = 0; i < repeat; ++i)
        {
            map.Step();
        }
        map.Print();
    }
    else
    {
        int loopingCount = 0;
        
        for (int row = 0; row < size.m_Y; ++row)
        {
            for (int col = 0; col < size.m_X; ++col)
            {
                if (map.Get({col, row}) == '.')
                {
                    GameMap copy = map;
                    copy.PutObstacle({col, row});
                    if (copy.StepUntilEnd() == GameMap::StepResult::Looping)
                    {
                        ++loopingCount;
                    }
                }
            }
        }
    
        map.StepUntilEnd();
        map.Print();

        std::cout << "Step count: " << map.CountMoved() << '\n';
        std::cout << "Looping count: " << loopingCount << '\n';
    }

    return 0;
}
