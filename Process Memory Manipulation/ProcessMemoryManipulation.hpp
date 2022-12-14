//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_PROCESSMEMORYMANIPULATION_HPP
#define DEBUGGER_PROCESSMEMORYMANIPULATION_HPP

#include <Windows.h>
#include <string>

#include "Utils/Utils.hpp"

class ProcessMemoryManipulation
{
public:
    static std::string ReadString(DWORD processId, ULONG_PTR address);

    static std::wstring ReadWString(DWORD processId, ULONG_PTR address);

    static ULONG_PTR ReadPointer(DWORD processId, ULONG_PTR address);

    static void WriteBytes(DWORD processId, ULONG_PTR address, const std::string& bytes);

    static void WriteBytes(DWORD processId, ULONG_PTR address, BYTE* bytes, size_t size);

    static void ReadBytes(DWORD processId, ULONG_PTR address, BYTE* buffer, size_t size);

    static ULONG_PTR GetProcAddressEx(HANDLE hProcess, ULONG_PTR moduleBase, const std::string& functionName, bool isWow64);

private:
    static ULONG_PTR GetProcAddressEx32(HANDLE hProcess, ULONG_PTR moduleBase, const std::string& function);

    static ULONG_PTR GetProcAddressEx64(HANDLE hProcess, ULONG_PTR moduleBase, const std::string& function);
};


#endif //DEBUGGER_PROCESSMEMORYMANIPULATION_HPP
