#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SquareMatrix.h"

constexpr std::size_t STRIDE = 140;

constexpr const char* SEARCH_WORD = "XMAS";
constexpr const char* SEARCH_WORD2 = "MAS";

int CountXmas(SquareMatrix matrix)
{
    int count = 0;
    int word_length = strlen(SEARCH_WORD);

    count += matrix.CountRows(SEARCH_WORD);
    matrix.Transpose();
    count += matrix.CountRows(SEARCH_WORD);

    for (size_t row = 0; row <= (STRIDE - word_length); ++row)
    {
        for (size_t col = 0; col <= (STRIDE - word_length); ++col)
        {
            auto window = matrix.ExtractWindow(col, row, word_length);
            int diagonal_count = window.CountDiagonals(SEARCH_WORD);

            count += diagonal_count;
        }
    }

    return count;
}

int CountX_mas(const SquareMatrix& matrix)
{
    int count = 0;
    int word_length = strlen(SEARCH_WORD2);

    for (size_t row = 0; row <= (STRIDE - word_length); ++row)
    {
        for (size_t col = 0; col <= (STRIDE - word_length); ++col)
        {
            auto window = matrix.ExtractWindow(col, row, word_length);
            if (window.CheckDiagonals(SEARCH_WORD2))
            {
                ++count;
            }
        }
    }

    return count;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
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

    SquareMatrix matrix{std::vector<char>{streamIt, std::istream_iterator<char>()}, STRIDE};

    int xmas_count = CountXmas(matrix);
    int x_mas_count = CountX_mas(matrix);

    std::cout << "XMAS: " << xmas_count << " X-MAS: " << x_mas_count << '\n';

    return 0;
}
