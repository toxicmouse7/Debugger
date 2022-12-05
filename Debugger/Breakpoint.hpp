//
// Created by Aleksej on 04.12.2022.
//

#ifndef DEBUGGER_BREAKPOINT_HPP
#define DEBUGGER_BREAKPOINT_HPP


#include "Process Memory Manipulation/ProcessMemoryManipulation.hpp"

class Breakpoint
{
private:
    ULONG_PTR address;
    DWORD pid;
    bool isEnabled;
    BYTE savedByte = 0;

public:
    explicit Breakpoint(DWORD pid, ULONG_PTR address);

    ~Breakpoint() = default;

    [[nodiscard]] unsigned __int64 GetAddress() const;

    [[nodiscard]] DWORD GetProcessId() const;

    [[nodiscard]] bool IsEnabled() const;

    void Enable();

    void Disable();
};

#endif //DEBUGGER_BREAKPOINT_HPP
