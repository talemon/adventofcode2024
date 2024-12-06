#include "state_machine.h"

#include <filesystem>
#include <string>


machine_state::machine_state(const std::string_view& name)
    : m_name(name)
{
}

void machine_state::Reset()
{
    OnReset();
}

void machine_state::AddEdge(machine_state* state)
{
    m_edges.push_back(state);
}

machine_state* machine_state::GetNextState(char ch)
{
    auto itr = std::ranges::find_if(
        m_edges,
        [ch](machine_state* state) -> bool
        {
            return state->Step(ch) != nullptr;
        });
    if (itr != m_edges.end())
    {
        return *itr;
    }
    else
    {
        if (m_default_edge != nullptr)
        {
            return m_default_edge->Step(ch);
        }
        return nullptr;
    }
}

void machine_state::InvokeOnComplete()
{
    if (m_on_complete.has_value())
    {
        std::invoke(m_on_complete.value());
    }
}

machine_state* any_state::Step(char ch)
{
    auto nextState = GetNextState(ch);
    return nextState != nullptr ? nextState : this;
}

machine_state* string_state::Step(char ch)
{
    if (current == m_Pattern.end())
    {
        InvokeOnComplete();
        return GetNextState(ch);
    }
    
    if (*current == ch)
    {
        ++current;
        return this;
    }
    else
    {
        return nullptr;
    }
}

void string_state::OnReset()
{
    current = m_Pattern.begin();
}

std::string string_state::DebugGetState() const
{
    if (current == m_Pattern.end())
    {
        return m_name + " completed";
    }
    else
    {
        return m_name + " at " + *current;
    }
}

machine_state* number_state::Step(char ch)
{
    if (std::isdigit(ch))
    {
        m_Number.push_back(ch);
        return this;
    }

    if (!m_Number.empty())
    {
        return GetNextState(ch);
    }

    return nullptr;
}

int number_state::Get() const
{
    return std::stoi(m_Number);
}

void number_state::OnReset()
{
    m_Number.clear();
}

std::string number_state::DebugGetState() const
{
    return m_name + " : " + m_Number;
}