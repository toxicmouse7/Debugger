//
// Created by Aleksej on 02.12.2022.
//

#include "ProcessMemoryManipulation.hpp"

#include <sstream>
#include <stdexcept>

std::string ProcessMemoryManipulation::ReadString(DWORD processId, ULONG_PTR address)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_READ, FALSE, processId);
    std::ostringstream result;
    char buffer[1024];
    SIZE_T bytesRead;

    while (true)
    {
        if (!ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), buffer, 1024, &bytesRead))
        {
            CloseHandle(processHandle);
            throw std::runtime_error("Failed to read process memory (" + std::to_string(GetLastError()) + ")");
        }
        for (int i = 0; i < bytesRead; i++)
        {
            if (buffer[i] == '\0')
            {
                CloseHandle(processHandle);
                return result.str();
            }
            result << buffer[i];
        }
        address += bytesRead;
    }
}

std::wstring ProcessMemoryManipulation::ReadWString(DWORD processId, ULONG_PTR address)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_READ, FALSE, processId);
    std::wostringstream result;
    wchar_t buffer[1024];
    SIZE_T bytesRead;

    while (true)
    {
        if (!ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), buffer, 1024, &bytesRead))
        {
            CloseHandle(processHandle);
            throw std::runtime_error("Failed to read process memory (" + std::to_string(GetLastError()) + ")");
        }
        for (int i = 0; i < bytesRead / 2; i++)
        {
            if (buffer[i] == L'\0')
            {
                CloseHandle(processHandle);
                return result.str();
            }
            result << buffer[i];
        }
        address += bytesRead;
    }
}

ULONG_PTR ProcessMemoryManipulation::ReadPointer(DWORD processId, ULONG_PTR address)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_READ, FALSE, processId);
    ULONG_PTR result;
    SIZE_T bytesRead;

    if (!ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &result, sizeof(void*), &bytesRead))
    {
        CloseHandle(processHandle);
        throw std::runtime_error("Failed to read process memory (" + std::to_string(GetLastError()) + ")");
    }

    CloseHandle(processHandle);

    return result;
}

void ProcessMemoryManipulation::WriteBytes(DWORD processId, ULONG_PTR address, const std::string& bytes)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
    SIZE_T bytesWritten;

    if (!WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), bytes.c_str(), bytes.length(),
                            &bytesWritten))
    {
        CloseHandle(processHandle);
        throw std::runtime_error("Failed to write process memory (" + std::to_string(GetLastError()) + ")");
    }

    CloseHandle(processHandle);
}

void ProcessMemoryManipulation::ReadBytes(DWORD processId, ULONG_PTR address, BYTE* buffer, size_t size)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_READ, FALSE, processId);
    SIZE_T bytesRead;

    if (!ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), buffer, size, &bytesRead))
    {
        CloseHandle(processHandle);
        throw std::runtime_error("Failed to read process memory (" + std::to_string(GetLastError()) + ")");
    }

    CloseHandle(processHandle);
}

void ProcessMemoryManipulation::WriteBytes(DWORD processId, ULONG_PTR address, BYTE* bytes, size_t size)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
    SIZE_T bytesWritten;

    if (!WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), bytes, size, &bytesWritten))
    {
        CloseHandle(processHandle);
        throw std::runtime_error("Failed to write process memory (" + std::to_string(GetLastError()) + ")");
    }

    CloseHandle(processHandle);
}
