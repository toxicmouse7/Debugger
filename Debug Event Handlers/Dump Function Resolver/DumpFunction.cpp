//
// Created by Aleksej on 06.12.2022.
//

#include "DumpFunction.hpp"
#include "Debugger/Debugger.hpp"

void DumpFunction::HandleDebugEvent(const ExceptionDebugEvent& event)
{
    auto thread = OpenThread(THREAD_SET_CONTEXT | THREAD_GET_CONTEXT, FALSE, event.threadId);
    if (thread == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to open thread");
    }

    CONTEXT context;
    auto debugger = Debugger::GetInstance();
    context.ContextFlags = CONTEXT_ALL;
    if (!GetThreadContext(thread, &context))
    {
        throw std::runtime_error("Failed to get thread context");
    }

    context.Rip--;

    BOOL isWow64 = false;
    auto process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, event.processId);
    IsWow64Process(process, &isWow64);
    CloseHandle(process);

    ULONG_PTR callAddress = 0;
    if (isWow64)
    {
        ProcessMemoryManipulation::ReadBytes(event.processId, context.Rsp, reinterpret_cast<BYTE*>(&callAddress),
                                             sizeof(ULONG));
    }
    else
    {
        ProcessMemoryManipulation::ReadBytes(event.processId, context.Rsp, reinterpret_cast<BYTE*>(&callAddress),
                                             sizeof(ULONG_PTR));
    }

    context.Dr6 = 0;

    if (!SetThreadContext(thread, &context))
    {
        throw std::runtime_error("Failed to set thread context");
    }

    CloseHandle(thread);
    ZydisDisassembledInstruction instruction;
    Utils::DisassembleInstruction(event.processId, context.Rip, isWow64, &instruction);
    if (instruction.info.mnemonic == ZYDIS_MNEMONIC_RET)
    {
        std::ranges::for_each(*retBreakpoints, [&debugger](const auto& address)
        {
            debugger->RemoveBreakpoint(address);
        });
        retBreakpoints.reset();
        LogFunctionReturn(event.processId, lastAddress, context.Rsp, isWow64, context.Rax);
        return;
    }

    auto function = Utils::DisassembleFunction(event.processId, context.Rip, isWow64);
    retBreakpoints = function.GetAddressesOfReturn();
    std::ranges::for_each(*retBreakpoints, [&debugger](const auto& address)
    {
        debugger->SetBreakpoint(address);
    });
    //debugger->SetBreakpoint(function->back().runtime_address);

    LogFunctionCall(event.processId, context.Rip, callAddress, context.Rsp, isWow64);
    lastAddress = context.Rip;
}

void DumpFunction::LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                                ULONG_PTR exceptionAddress) const
{

}

void DumpFunction::LogFunctionCall(DWORD pid, ULONG_PTR functionAddress, ULONG_PTR callAddress, ULONG_PTR stackAddress,
                                   bool isWow64) const
{
    auto functionsDatabase = FunctionsDatabase::GetInstance();
    if (GetLogger().has_value())
    {
        std::wstringstream ss;
        ss << functionsDatabase->GetFunctionName(functionAddress).c_str() << " call at ";
        ss << std::hex << Utils::FormatAddress(callAddress, "'").c_str() << std::endl;

        auto argInfo = functionsDatabase->GetFunctionArgs(functionsDatabase->GetFunctionName(functionAddress));
        ss << ParseArgs(pid, argInfo, isWow64, stackAddress);

        GetLogger().value().get().Log(ss.str());
    }
}

std::wstring DumpFunction::ParseArgs(DWORD pid, const ArgInfo& argInfo, bool isWow64, ULONG_PTR stackAddress)
{
    std::wstringstream ss;

    for (int i = 0; i < argInfo.count; i++)
    {
        ULONG_PTR argValue = 0;
        if (isWow64)
        {
            ProcessMemoryManipulation::ReadBytes(pid, stackAddress + 4 + i * 4, reinterpret_cast<BYTE*>(&argValue),
                                                 sizeof(ULONG));
        }
        else
        {
            ProcessMemoryManipulation::ReadBytes(pid, stackAddress + 8 + i * 8, reinterpret_cast<BYTE*>(&argValue),
                                                 sizeof(ULONG_PTR));
        }

        ss << argInfo.names[i].c_str() << ' ' << argInfo.values[i].c_str() << ": ";

        if (ArgInfo::IsPointer(argInfo.types[i]) && argValue == 0)
        {
            ss << argValue << std::endl;
            continue;
        }

        switch (argInfo.types[i])
        {
            case ArgType::stringPointer:
                ss << std::hex << Utils::FormatAddress(argValue, "'").c_str();
                ss << " (" << ProcessMemoryManipulation::ReadString(pid, argValue).c_str() << ')' << std::endl;
                break;
            case ArgType::wideStringPointer:
                ss << std::hex << Utils::FormatAddress(argValue, "'").c_str();
                ss << L" (" << ProcessMemoryManipulation::ReadWString(pid, argValue) << L')' << std::endl;
                break;
            case ArgType::structurePointer:
                ss << ParseStructure(pid, argInfo.names[i], isWow64, argValue, 0);
                break;
            case ArgType::bitField:
                ss << argValue << ParseBitfield(argInfo.values[i], argValue, 0);
                break;
            case ArgType::pointer:
                ss << std::hex << Utils::FormatAddress(argValue, "'").c_str() << std::endl;
                break;
            case ArgType::number:
            default:
                ss << argValue << std::endl;
                break;
        }
    }

    return ss.str();
}

std::wstring DumpFunction::ParseStructure(DWORD pid, std::string structureName, bool isWow64,
                                          ULONG_PTR structureAddress,
                                          uint32_t nestingLevel)
{
    nestingLevel++;
    if (structureName.starts_with("LP"))
    {
        structureName = structureName.substr(2);
    }

    auto structuresDatabase = StructuresDatabase::GetInstance();
    ArgInfo structureInfo;
    std::wstringstream ss;
    uint32_t structureOffset = 0;

    ss << '\n';

    try
    {
        structureInfo = structuresDatabase->GetStructure(structureName);
    }
    catch (const std::exception& e)
    {
        AddNesting(ss, nestingLevel);
        ss << "Unknown structure";
        return ss.str();
    }

    for (int i = 0; i < structureInfo.count; ++i)
    {
        ULONG_PTR argValue = 0;

        ProcessMemoryManipulation::ReadBytes(pid, structureAddress + structureOffset,
                                             reinterpret_cast<BYTE*>(&argValue),
                                             structureInfo.sizes[i]);

        AddNesting(ss, nestingLevel);
        ss << structureInfo.names[i].c_str() << ' ' << structureInfo.values[i].c_str() << ": ";

        if (ArgInfo::IsPointer(structureInfo.types[i]) && argValue == 0)
        {
            ss << argValue << std::endl;
            continue;
        }

        switch (structureInfo.types[i])
        {
            case ArgType::structurePointer:
                ss << ParseStructure(pid, structureInfo.names[i], isWow64,
                                     structureAddress + structureOffset, nestingLevel).c_str();
                break;
            case ArgType::stringPointer:
                ss << std::hex << Utils::FormatAddress(argValue, "'").c_str();
                ss << " (" << ProcessMemoryManipulation::ReadString(pid, argValue).c_str() << ')' << std::endl;
                break;
            case ArgType::wideStringPointer:
                ss << std::hex << Utils::FormatAddress(argValue, "'").c_str();
                ss << L" (" << ProcessMemoryManipulation::ReadWString(pid, argValue) << L')' << std::endl;
                break;
            case ArgType::number:
                switch (structureInfo.sizes[i])
                {
                    case 1:
                        ss << static_cast<uint8_t>(argValue) << std::endl;
                        break;
                    case 2:
                        ss << static_cast<uint16_t>(argValue) << std::endl;
                        break;
                    case 4:
                        ss << static_cast<uint32_t>(argValue) << std::endl;
                        break;
                    case 8:
                        ss << static_cast<uint64_t>(argValue) << std::endl;
                        break;
                    default:
                        break;
                }
                break;
            case ArgType::bitField:
                ss << argValue << ParseBitfield(structureInfo.values[i], argValue, nestingLevel);
                break;
            case ArgType::pointer:
                ss << std::hex << Utils::FormatAddress(argValue, "'").c_str() << std::endl;
                break;
            default:
                ss << argValue << std::endl;
                break;
        }

        structureOffset += structureInfo.sizes[i];
    }

    return ss.str();
}

std::wstring DumpFunction::ParseBitfield(const std::string& bitfieldName, ULONG_PTR bitfieldValue,
                                         uint32_t nestingLevel)
{
    nestingLevel++;
    auto bitfieldsDatabase = BitfieldsDatabase::GetInstance();
    ArgInfo bitfieldInfo;
    std::wstringstream ss;

    ss << '\n';

    try
    {
        bitfieldInfo = bitfieldsDatabase->GetBitfield(bitfieldName);
    }
    catch (const std::exception& e)
    {
        AddNesting(ss, nestingLevel);
        ss << "Unknown bitfield";
        return ss.str();
    }

    for (int i = 0; i < bitfieldInfo.count; ++i)
    {
        if (bitfieldValue & bitfieldInfo.sizes[i])
        {
            AddNesting(ss, nestingLevel);
            ss << bitfieldInfo.values[i].c_str() << std::endl;
        }
    }

    return ss.str();
}

void DumpFunction::AddNesting(std::wstringstream& ss, uint32_t nesting)
{
    for (int i = 0; i < nesting; ++i)
    {
        ss << "     ";
    }
}

void DumpFunction::LogFunctionReturn(DWORD pid, ULONG_PTR functionAddress, ULONG_PTR stackAddress, bool isWow64,
                                     ULONG_PTR returnValue) const
{
    auto functionsDatabase = FunctionsDatabase::GetInstance();
    if (GetLogger().has_value())
    {
        std::wstringstream ss;
        ss << "Return value: " << Utils::FormatAddress(returnValue, "'").c_str() << std::endl;

        auto argInfo = functionsDatabase->GetFunctionArgs(functionsDatabase->GetFunctionName(functionAddress));
        ss << ParseArgs(pid, argInfo, isWow64, stackAddress);

        GetLogger().value().get().Log(ss.str());
    }
}
