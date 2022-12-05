//
// Created by Aleksej on 04.12.2022.
//

#include "Breakpoint.hpp"

Breakpoint::Breakpoint(DWORD pid, unsigned __int64 address) : address(address), pid(pid)
{
    ProcessMemoryManipulation::ReadBytes(pid, address, &savedByte, 1);
    ProcessMemoryManipulation::WriteBytes(pid, address, "\xCC");
    isEnabled = true;
}

unsigned __int64 Breakpoint::GetAddress() const
{
    return address;
}

bool Breakpoint::IsEnabled() const
{
    return isEnabled;
}

DWORD Breakpoint::GetProcessId() const
{
    return pid;
}

void Breakpoint::Disable()
{
    if (isEnabled)
    {
        ProcessMemoryManipulation::WriteBytes(pid, address, &savedByte, 1);
        isEnabled = false;
    }
}

void Breakpoint::Enable()
{
    if (!isEnabled)
    {
        ProcessMemoryManipulation::WriteBytes(pid, address, "\xCC");
        isEnabled = true;
    }
}
