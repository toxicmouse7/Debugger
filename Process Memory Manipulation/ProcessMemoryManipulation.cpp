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
    ULONG_PTR result = 0;
    SIZE_T bytesRead;
    auto pointerSize = Utils::IsWow64(processId) ? 4 : 8;

    if (!ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &result, pointerSize, &bytesRead))
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

ULONG_PTR ProcessMemoryManipulation::GetProcAddressEx(HANDLE hProcess, ULONG_PTR moduleBase,
                                                      const std::string& functionName, bool isWow64)
{
    if (isWow64)
        return GetProcAddressEx32(hProcess, moduleBase, functionName);

    return GetProcAddressEx64(hProcess, moduleBase, functionName);
}

ULONG_PTR ProcessMemoryManipulation::GetProcAddressEx32(HANDLE hProcess, ULONG_PTR moduleBase,
                                                        const std::string& function)
{
    IMAGE_DOS_HEADER imageDosHeader = {0};

    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase), &imageDosHeader, sizeof(IMAGE_DOS_HEADER),
                           nullptr))
        return 0;

    if (imageDosHeader.e_magic != IMAGE_DOS_SIGNATURE)
        return 0;

    IMAGE_NT_HEADERS32 imageNtHeaders = {0};

    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase + imageDosHeader.e_lfanew), &imageNtHeaders,
                           sizeof(IMAGE_NT_HEADERS32), nullptr))
        return 0;

    if (imageNtHeaders.Signature != IMAGE_NT_SIGNATURE)
        return 0;

    IMAGE_EXPORT_DIRECTORY imageExportDirectory = {0};
    uintptr_t img_exp_dir_rva;

    if (!(img_exp_dir_rva = imageNtHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress))
        return 0;

    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase + img_exp_dir_rva), &imageExportDirectory,
                           sizeof(IMAGE_EXPORT_DIRECTORY), nullptr))
        return 0;

    uintptr_t EAT = moduleBase + imageExportDirectory.AddressOfFunctions;
    uintptr_t ENT = moduleBase + imageExportDirectory.AddressOfNames;
    uintptr_t EOT = moduleBase + imageExportDirectory.AddressOfNameOrdinals;

    WORD ordinal = 0;
    size_t bufferSize = function.size() + 1;
    char* tempBuf = new char[bufferSize];

    for (size_t i = 0; i < imageExportDirectory.NumberOfNames; i++)
    {
        uintptr_t tempRvaString = 0;

        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(ENT + (i * sizeof(uint32_t))), &tempRvaString,
                               sizeof(uint32_t), nullptr))
            return 0;

        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase + tempRvaString), tempBuf, bufferSize,
                               nullptr))
            return 0;

        if (!lstrcmpi(function.c_str(), tempBuf))
        {
            if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(EOT + (i * sizeof(WORD))), &ordinal,
                                   sizeof(WORD), nullptr))
                return 0;

            uintptr_t temp_rva_func = 0;

            if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(EAT + (ordinal * sizeof(uint32_t))),
                                   &temp_rva_func, sizeof(uint32_t), nullptr))
                return 0;

            delete[] tempBuf;
            return moduleBase + temp_rva_func;
        }
    }
    delete[] tempBuf;
    return 0;
}

ULONG_PTR ProcessMemoryManipulation::GetProcAddressEx64(HANDLE hProcess, ULONG_PTR moduleBase,
                                                        const std::string& function)
{
    IMAGE_DOS_HEADER imageDosHeader = {0};

    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase), &imageDosHeader, sizeof(IMAGE_DOS_HEADER),
                           nullptr))
        return 0;

    if (imageDosHeader.e_magic != IMAGE_DOS_SIGNATURE)
        return 0;

    IMAGE_NT_HEADERS imageNtHeaders = {0};

    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase + imageDosHeader.e_lfanew), &imageNtHeaders,
                           sizeof(IMAGE_NT_HEADERS), nullptr))
        return 0;

    if (imageNtHeaders.Signature != IMAGE_NT_SIGNATURE)
        return 0;

    IMAGE_EXPORT_DIRECTORY imageExportDirectory = {0};
    uintptr_t img_exp_dir_rva;

    if (!(img_exp_dir_rva = imageNtHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress))
        return 0;

    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase + img_exp_dir_rva), &imageExportDirectory,
                           sizeof(IMAGE_EXPORT_DIRECTORY), nullptr))
        return 0;

    uintptr_t EAT = moduleBase + imageExportDirectory.AddressOfFunctions;
    uintptr_t ENT = moduleBase + imageExportDirectory.AddressOfNames;
    uintptr_t EOT = moduleBase + imageExportDirectory.AddressOfNameOrdinals;

    WORD ordinal = 0;
    size_t bufferSize = function.size() + 1;
    char* tempBuf = new char[bufferSize];

    for (size_t i = 0; i < imageExportDirectory.NumberOfNames; i++)
    {
        uintptr_t tempRvaString = 0;

        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(ENT + (i * sizeof(uintptr_t))), &tempRvaString,
                               sizeof(uintptr_t), nullptr))
            return 0;

        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(moduleBase + tempRvaString), tempBuf, bufferSize,
                               nullptr))
            return 0;

        if (!lstrcmpi(function.c_str(), tempBuf))
        {
            if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(EOT + (i * sizeof(WORD))), &ordinal,
                                   sizeof(WORD), nullptr))
                return 0;

            uintptr_t temp_rva_func = 0;

            if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(EAT + (ordinal * sizeof(uintptr_t))),
                                   &temp_rva_func, sizeof(uintptr_t), nullptr))
                return 0;

            delete[] tempBuf;
            return moduleBase + temp_rva_func;
        }
    }
    delete[] tempBuf;
    return 0;
}
