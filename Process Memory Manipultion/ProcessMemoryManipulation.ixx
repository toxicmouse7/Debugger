//
// Created by Aleksej on 02.12.2022.
//

module;

#include <Windows.h>

export module ProcessMemoryManipulation;

import std.core;

export class ProcessMemoryManipulation
{
public:
    static std::string ReadString(HANDLE processHandle, LPVOID address);
    static std::wstring ReadWString(HANDLE processHandle, LPVOID address);
    static void* ReadPointer(HANDLE processHandle, LPVOID address);
};

std::string ProcessMemoryManipulation::ReadString(HANDLE processHandle, LPVOID address)
{
    std::ostringstream result;
    char buffer[1024];
    SIZE_T bytesRead;

    while (true)
    {
        if (!ReadProcessMemory(processHandle, address, buffer, 1024, &bytesRead))
        {
            throw std::runtime_error("Failed to read process memory (" + std::to_string(GetLastError()) + ")");
        }
        for (int i = 0; i < bytesRead; i++)
        {
            if (buffer[i] == '\0')
            {
                return result.str();
            }
            result << buffer[i];
        }
        address = reinterpret_cast<LPVOID>(reinterpret_cast<ULONG_PTR>(address) + bytesRead);
    }
}

std::wstring ProcessMemoryManipulation::ReadWString(HANDLE processHandle, LPVOID address)
{
    std::wostringstream result;
    wchar_t buffer[1024];
    SIZE_T bytesRead;

    while (true)
    {
        if (!ReadProcessMemory(processHandle, address, buffer, 1024, &bytesRead))
        {
            throw std::runtime_error("Failed to read process memory (" + std::to_string(GetLastError()) + ")");
        }
        for (int i = 0; i < bytesRead / 2; i++)
        {
            if (buffer[i] == L'\0')
            {
                return result.str();
            }
            result << buffer[i];
        }
        address = reinterpret_cast<LPVOID>(reinterpret_cast<ULONG_PTR>(address) + bytesRead);
    }
}

void* ProcessMemoryManipulation::ReadPointer(HANDLE processHandle, LPVOID address)
{
    void* result;
    SIZE_T bytesRead;

    if (!ReadProcessMemory(processHandle, address, &result, sizeof(void*), &bytesRead))
    {
        throw std::runtime_error("Failed to read process memory (" + std::to_string(GetLastError()) + ")");
    }

    return result;
}