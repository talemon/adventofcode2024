#pragma once

#include <cassert>
#include <functional>
#include <optional>
#include <string>

struct machine_state
{
    virtual ~machine_state() = default;
    machine_state(const std::string_view& name);
    
    virtual machine_state* Step(char ch) = 0;

    virtual void OnReset()
    {
    }

    virtual std::string DebugGetState() const { return m_name; }

    void Reset();

    void AddEdge(machine_state* state);
    machine_state* GetNextState(char ch);

    void SetDefaultEdge(machine_state* state) { m_default_edge = state; }
    void OnComplete(const std::function<void()>& func) { m_on_complete = func; }
    void InvokeOnComplete();

protected:
    std::string m_name;
    std::optional<std::function<void()>> m_on_complete;
    machine_state* m_default_edge = nullptr;
    std::vector<machine_state*> m_edges;
};

struct any_state : machine_state
{
    explicit any_state(const std::string_view& name)
        : machine_state(name)
    {
    }

    machine_state* Step(char ch) override;
};

struct string_state : machine_state
{
    explicit string_state(const std::string_view& name, std::string pattern)
        : machine_state(name), m_Pattern(std::move(pattern))
    {
        assert(!m_Pattern.empty());

        current = m_Pattern.cbegin();
    }

    machine_state* Step(char ch) override;
    void OnReset() override;
    std::string DebugGetState() const override;

private:
    std::string m_Pattern;
    std::string::const_iterator current;
};

struct number_state : machine_state
{
    explicit number_state(const std::string_view& name)
        : machine_state(name)
    {
    }

    machine_state* Step(char ch) override;
    void OnReset() override;
    std::string DebugGetState() const override;

    int Get() const;
    bool Empty() const { return m_Number.empty(); }

private:
    std::string m_Number;
};
