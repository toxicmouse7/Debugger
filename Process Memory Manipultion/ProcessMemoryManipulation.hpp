//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_PROCESSMEMORYMANIPULATION_HPP
#define DEBUGGER_PROCESSMEMORYMANIPULATION_HPP

#include <Windows.h>
#include <string>

class ProcessMemoryManipulation
{
public:
    static std::string ReadString(HANDLE processHandle, LPVOID address);
    static std::wstring ReadWString(HANDLE processHandle, LPVOID address);
    static void* ReadPointer(HANDLE processHandle, LPVOID address);
};


#endif //DEBUGGER_PROCESSMEMORYMANIPULATION_HPP
