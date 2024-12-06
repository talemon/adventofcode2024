#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "state_machine.h"

// #define DEBUG_ENABLED

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
        std::cerr << "Unable to open file " << argv[0] << '\n';
        return 1;
    }
    file.unsetf(std::ios::skipws);

    any_state start_state{"start"};

    string_state multiply_state{"multiply_state", "mul("};
    number_state number1{"number1"};
    string_state multiply_separator_state{"mul_separator", ","};
    number_state number2{"number2"};
    string_state mul_suffix{"mul_suffix", ")"};

    string_state do_prefix{"do_prefix", "do"};
    string_state do_state{"do_state", "()"};
    string_state dont_state{"dont_state", "n't()"};

    std::vector<machine_state*> state_list = {
        &start_state,
        &multiply_state,
        &number1,
        &multiply_separator_state,
        &number2,
        &mul_suffix,
        &do_prefix,
        &do_state,
        &dont_state
    };

    start_state.AddEdge(&multiply_state);
    start_state.AddEdge(&do_prefix);

    multiply_state.SetDefaultEdge(&number1);
    number1.SetDefaultEdge(&multiply_separator_state);
    multiply_separator_state.SetDefaultEdge(&number2);
    number2.SetDefaultEdge(&mul_suffix);

    long total = 0;
    int counter = 0;
    bool mul_enabled = true;

    mul_suffix.OnComplete([&]()
    {
        if (!mul_enabled)
            return;

        int n1 = number1.Get();
        int n2 = number2.Get();

        std::cout << "Multiplying: " << n1 << " X " << n2 << '\n';
        total += n1 * n2;
        ++counter;
    });

    do_prefix.AddEdge(&do_state);
    do_prefix.AddEdge(&dont_state);

    do_state.OnComplete([&]()
    {
        mul_enabled = true;
    });

    dont_state.OnComplete([&]()
    {
        mul_enabled = false;
    });

    machine_state* current_state = &start_state;

    while (!file.eof())
    {
        char ch = static_cast<char>(file.get());

#ifdef DEBUG_ENABLED
        std::cout << "Stepping with " << ch << " on: " << current_state->DebugGetState() << "\n";
#endif
        machine_state* result = current_state->Step(ch);

        if (result == nullptr)
        {
            std::ranges::for_each(
                state_list,
                &machine_state::Reset);

            result = start_state.Step(ch);
        }

        current_state = result;
#ifdef DEBUG_ENABLED
        std::cout << "New current: " << current_state->DebugGetState() << "\n";
#endif
    }

    std::cout << "Total: " << total << " count: " << counter << '\n';

    return 0;
}
