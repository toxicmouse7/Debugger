#include "Debugger/Debugger.hpp"
#include "Debug Event Handlers/Default/DefaultHandlers.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Message Handlers/MessageHandlers.hpp"
#include "Debug Event Handlers/TraceResolver.hpp"
#include "Debug Event Handlers/Dump Function Resolver/DumpFunction.hpp"
#include "Debug Event Handlers/Dump Function Resolver/FunctionsDatabase.hpp"
#include "Debug Event Handlers/Dump Function Resolver/StructuresDatabase.hpp"
#include "Debug Event Handlers/Dump Function Resolver/BitfieldsDatabase.hpp"
#include "Debug Event Handlers/Trace Functions Resolver/TraceFunctionsResolver.hpp"

void AddFunctionsToDatabase(bool isWow64)
{
    auto database = FunctionsDatabase::GetInstance();
    auto debugger = Debugger::GetInstance();
    auto pointerSize = isWow64 ? 4 : 8;

    ArgInfo exitProcessArgs;
    exitProcessArgs.AddArg(ArgType::number, "UINT", "uExitCode", sizeof(UINT));
    auto exitProcessAddress = debugger->GetProcAddress(L"kernel32.dll", "ExitProcess");
    database->AddFunction("ExitProcess", exitProcessAddress, exitProcessArgs);

    ArgInfo terminateProcessArgs;
    terminateProcessArgs.AddArg(ArgType::pointer, "HANDLE", "hProcess", pointerSize);
    terminateProcessArgs.AddArg(ArgType::number, "UINT", "uExitCode", sizeof(UINT));
    auto terminateProcessAddress = debugger->GetProcAddress(L"kernel32.dll", "TerminateProcess");
    database->AddFunction("TerminateProcess", terminateProcessAddress, terminateProcessArgs);

    ArgInfo createProcessAArgs;
    createProcessAArgs.AddArg(ArgType::stringPointer, "LPCSTR", "lpApplicationName", pointerSize);
    createProcessAArgs.AddArg(ArgType::stringPointer, "LPSTR", "lpCommandLine", pointerSize);
    createProcessAArgs.AddArg(ArgType::structurePointer, "LPSECURITY_ATTRIBUTES", "lpProcessAttributes", pointerSize);
    createProcessAArgs.AddArg(ArgType::structurePointer, "LPSECURITY_ATTRIBUTES", "lpThreadAttributes", pointerSize);
    createProcessAArgs.AddArg(ArgType::number, "BOOL", "bInheritHandles", sizeof(BOOL));
    createProcessAArgs.AddArg(ArgType::bitField, "DWORD", "dwCreationFlags", sizeof(DWORD));
    createProcessAArgs.AddArg(ArgType::pointer, "LPVOID", "lpEnvironment", pointerSize);
    createProcessAArgs.AddArg(ArgType::stringPointer, "LPCSTR", "lpCurrentDirectory", pointerSize);
    createProcessAArgs.AddArg(ArgType::structurePointer, "LPSTARTUPINFOA", "lpStartupInfo", pointerSize);
    createProcessAArgs.AddArg(ArgType::structurePointer, "LPPROCESS_INFORMATION", "lpProcessInformation", pointerSize);
    auto createProcessAAddress = debugger->GetProcAddress(L"kernel32.dll", "CreateProcessA");
    database->AddFunction("CreateProcessA", createProcessAAddress, createProcessAArgs);

    ArgInfo createProcessWArgs;
    createProcessWArgs.AddArg(ArgType::wideStringPointer, "LPCWSTR", "lpApplicationName", pointerSize);
    createProcessWArgs.AddArg(ArgType::wideStringPointer, "LPWSTR", "lpCommandLine", pointerSize);
    createProcessWArgs.AddArg(ArgType::structurePointer, "LPSECURITY_ATTRIBUTES", "lpProcessAttributes", pointerSize);
    createProcessWArgs.AddArg(ArgType::structurePointer, "LPSECURITY_ATTRIBUTES", "lpThreadAttributes", pointerSize);
    createProcessWArgs.AddArg(ArgType::number, "BOOL", "bInheritHandles", sizeof(BOOL));
    createProcessWArgs.AddArg(ArgType::bitField, "DWORD", "dwCreationFlags", sizeof(DWORD));
    createProcessWArgs.AddArg(ArgType::pointer, "LPVOID", "lpEnvironment", pointerSize);
    createProcessWArgs.AddArg(ArgType::wideStringPointer, "LPCWSTR", "lpCurrentDirectory", pointerSize);
    createProcessWArgs.AddArg(ArgType::structurePointer, "LPSTARTUPINFOW", "lpStartupInfo", pointerSize);
    createProcessWArgs.AddArg(ArgType::structurePointer, "LPPROCESS_INFORMATION", "lpProcessInformation", pointerSize);
    auto createProcessWAddress = debugger->GetProcAddress(L"kernel32.dll", "CreateProcessW");
    database->AddFunction("CreateProcessW", createProcessWAddress, createProcessWArgs);
}

void AddStructuresToDatabase(bool isWow64)
{
    auto structuresDatabase = StructuresDatabase::GetInstance();
    auto pointerSize = isWow64 ? 4 : 8;

    ArgInfo securityAttributes;
    securityAttributes.AddArg(ArgType::number, "DWORD", "nLength", sizeof(DWORD));
    securityAttributes.AddArg(ArgType::pointer, "LPVOID", "lpSecurityDescriptor", pointerSize);
    securityAttributes.AddArg(ArgType::number, "BOOL", "bInheritHandle", sizeof(BOOL));
    structuresDatabase->AddStructure("SECURITY_ATTRIBUTES", securityAttributes);

    ArgInfo startupInfoA;
    startupInfoA.AddArg(ArgType::number, "DWORD", "cb", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::stringPointer, "LPSTR", "lpReserved", pointerSize);
    startupInfoA.AddArg(ArgType::stringPointer, "LPSTR", "lpDesktop", pointerSize);
    startupInfoA.AddArg(ArgType::stringPointer, "LPSTR", "lpTitle", pointerSize);
    startupInfoA.AddArg(ArgType::number, "DWORD", "dwX", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::number, "DWORD", "dwY", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::number, "DWORD", "dwXSize", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::number, "DWORD", "dwYSize", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::number, "DWORD", "dwXCountChars", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::number, "DWORD", "dwYCountChars", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::number, "DWORD", "dwFillAttribute", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::bitField, "DWORD", "dwFlags", sizeof(DWORD));
    startupInfoA.AddArg(ArgType::number, "WORD", "wShowWindow", sizeof(WORD));
    startupInfoA.AddArg(ArgType::number, "WORD", "cbReserved2", sizeof(WORD));
    startupInfoA.AddArg(ArgType::pointer, "LPBYTE", "lpReserved2", pointerSize);
    startupInfoA.AddArg(ArgType::pointer, "HANDLE", "hStdInput", pointerSize);
    startupInfoA.AddArg(ArgType::pointer, "HANDLE", "hStdOutput", pointerSize);
    startupInfoA.AddArg(ArgType::pointer, "HANDLE", "hStdError", pointerSize);
    structuresDatabase->AddStructure("STARTUPINFOA", startupInfoA);

    ArgInfo startupInfoW;
    startupInfoW.AddArg(ArgType::number, "DWORD", "cb", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::wideStringPointer, "LPWSTR", "lpReserved", pointerSize);
    startupInfoW.AddArg(ArgType::wideStringPointer, "LPWSTR", "lpDesktop", pointerSize);
    startupInfoW.AddArg(ArgType::wideStringPointer, "LPWSTR", "lpTitle", pointerSize);
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwX", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwY", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwXSize", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwYSize", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwXCountChars", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwYCountChars", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwFillAttribute", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "DWORD", "dwFlags", sizeof(DWORD));
    startupInfoW.AddArg(ArgType::number, "WORD", "wShowWindow", sizeof(WORD));
    startupInfoW.AddArg(ArgType::number, "WORD", "cbReserved2", sizeof(WORD));
    startupInfoW.AddArg(ArgType::pointer, "LPBYTE", "lpReserved2", pointerSize);
    startupInfoW.AddArg(ArgType::pointer, "HANDLE", "hStdInput", pointerSize);
    startupInfoW.AddArg(ArgType::pointer, "HANDLE", "hStdOutput", pointerSize);
    startupInfoW.AddArg(ArgType::pointer, "HANDLE", "hStdError", pointerSize);
    structuresDatabase->AddStructure("STARTUPINFOW", startupInfoW);

    ArgInfo processInformation;
    processInformation.AddArg(ArgType::pointer, "HANDLE", "hProcess", pointerSize);
    processInformation.AddArg(ArgType::pointer, "HANDLE", "hThread", pointerSize);
    processInformation.AddArg(ArgType::number, "DWORD", "dwProcessId", sizeof(DWORD));
    processInformation.AddArg(ArgType::number, "DWORD", "dwThreadId", sizeof(DWORD));
    structuresDatabase->AddStructure("PROCESS_INFORMATION", processInformation);
}

void AddBitfieldsToDatabase()
{
    auto bitfieldsDatabase = BitfieldsDatabase::GetInstance();

    ArgInfo dwCreationFlags;
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_SUSPENDED", 0x00000004);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_NEW_CONSOLE", 0x00000010);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_NEW_PROCESS_GROUP", 0x00000200);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_UNICODE_ENVIRONMENT", 0x00000400);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_SEPARATE_WOW_VDM", 0x00000800);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_SHARED_WOW_VDM", 0x00001000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_FORCEDOS", 0x00002000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "BELOW_NORMAL_PRIORITY_CLASS", 0x00004000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "ABOVE_NORMAL_PRIORITY_CLASS", 0x00008000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "INHERIT_PARENT_AFFINITY", 0x00010000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "INHERIT_CALLER_PRIORITY", 0x00020000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_PROTECTED_PROCESS", 0x00040000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "EXTENDED_STARTUPINFO_PRESENT", 0x00080000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "PROCESS_MODE_BACKGROUND_BEGIN", 0x00100000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "PROCESS_MODE_BACKGROUND_END", 0x00200000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_BREAKAWAY_FROM_JOB", 0x01000000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_PRESERVE_CODE_AUTHZ_LEVEL", 0x02000000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_DEFAULT_ERROR_MODE", 0x04000000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_NO_WINDOW", 0x08000000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "PROFILE_USER", 0x10000000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "PROFILE_KERNEL", 0x20000000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "PROFILE_SERVER", 0x40000000);
    dwCreationFlags.AddArg(ArgType::bitField, "DWORD", "CREATE_IGNORE_SYSTEM_DEFAULT", 0x80000000);
    bitfieldsDatabase->AddBitfield("dwCreationFlags", dwCreationFlags);

    ArgInfo dwFlags;
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_USESHOWWINDOW", 0x00000001);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_USESIZE", 0x00000002);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_USEPOSITION", 0x00000004);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_USECOUNTCHARS", 0x00000008);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_USEFILLATTRIBUTE", 0x00000010);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_RUNFULLSCREEN", 0x00000020);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_FORCEONFEEDBACK", 0x00000040);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_FORCEOFFFEEDBACK", 0x00000080);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_USESTDHANDLES", 0x00000100);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_USEHOTKEY", 0x00000200);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_TITLEISLINKNAME", 0x00000800);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_TITLEISAPPID", 0x00001000);
    dwFlags.AddArg(ArgType::bitField, "DWORD", "STARTF_PREVENTPINNING", 0x00002000);
    bitfieldsDatabase->AddBitfield("dwFlags", dwFlags);
}

int main()
{
    auto debugger = Debugger::GetInstance();
    auto& logger = Logger::GetInstance("Debugger");

    //logger.AddMessageHandler(new ConsoleMessageHandler());
    logger.AddMessageHandler(new FileMessageHandler("debugger.log"));

    debugger->AddLoadDllHandler<DefaultLoadDllDebugEventHandler>(logger);
    debugger->AddCreateProcessHandler<DefaultCreateProcessDebugEventHandler>(logger);
    debugger->AddUnloadDllHandler<DefaultUnloadDllDebugEventHandler>(logger);
    debugger->AddExitProcessHandler<DefaultExitProcessDebugEventHandler>(logger);
    debugger->AddExitThreadHandler<DefaultExitThreadDebugEventHandler>(logger);
    debugger->AddCreateThreadHandler<DefaultCreateThreadDebugEventHandler>(logger);

    debugger->SetErrorLogger(logger);

    auto& debuggerState = debugger->CreateAndAttach(
            R"(C:\Users\Aleksej\CLionProjects\test123\cmake-build-debug-x86\test.exe)");
//    auto& debuggerState = debugger->CreateAndAttach(
//            R"(C:\Windows\notepad.exe)");

//    debugger->WaitForEntry(false);
//
    debugger->WaitForEntry(true);

//    AddFunctionsToDatabase(debugger->IsWow64());
//    AddStructuresToDatabase(debugger->IsWow64());
//    AddBitfieldsToDatabase();

//    debugger->SetBreakpoint(debugger->GetProcAddress(L"kernelbase.dll", "ExitProcess"));
//    debugger->SetBreakpoint(debugger->GetProcAddress(L"kernel32.dll", "CreateProcessA"));
//    debugger->SetBreakpoint(debugger->GetProcAddress(L"kernel32.dll", "TerminateProcess"));

    while (debuggerState != DebuggerState::Stopped)
    {
        auto exceptionContext = debugger->WaitForDebugEvent();

        if (exceptionContext.IsExternal() && exceptionContext.Type() != ExceptionContextType::eHardBreakpoint)
        {
            debugger->UseResolver<DefaultExceptionDebugEventHandler>(exceptionContext, logger);
        }
        else
        {
            debugger->UseResolver<TraceFunctionsResolver>(exceptionContext, logger);
           // debugger->UseResolver<DumpFunction>(exceptionContext, logger);
        }
    }

    return 0;
}
