cmake_minimum_required(VERSION 3.23)
project(Debugger)

set(CMAKE_CXX_STANDARD 20)

option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("deps/zydis")

add_executable(Debugger main.cpp
        Debugger/Debugger.hpp
        "Abstractions/Event Managing/AbstractEvent.hpp"
        "Abstractions/Event Managing/AbstractEventManager.hpp"
        "Abstractions/Event Managing/AbstractEventRecipient.hpp"
        "Abstractions/Message Handling/AbstractMessageHandler.hpp"
        "Abstractions/Debug Event Handlers/AbstractLoadDllDebugEventHandler.hpp"
        "Abstractions/Debug Events/AbstractDebugEvent.hpp" "Debug Events/LoadDllDebugEvent.hpp"
        Logger/Logger.cpp Logger/Logger.hpp
        "Logger/Message Handlers/ConsoleMessageHandler.hpp"
        "Process Memory Manipulation/ProcessMemoryManipulation.cpp"
        "Process Memory Manipulation/ProcessMemoryManipulation.hpp"
        Utils/Utils.cpp Utils/Utils.hpp
        "Debug Event Handlers/Default/DefaultLoadDllDebugEventHandler.cpp"
        "Debug Event Handlers/Default/DefaultLoadDllDebugEventHandler.hpp"
        "Debug Event Handlers/DebugEventHandlerType.hpp"
        "Debug Event Handlers/Default/DefaultHandlers.hpp"
        "Logger/Message Handlers/MessageHandlers.hpp"
        "Abstractions/Debug Event Handlers/AbstractCreateProcessDebugEventHandler.hpp"
        "Debug Events/CreateProcessDebugEvent.hpp"
        "Debug Event Handlers/Default/DefaultCreateProcessDebugEventHandler.cpp"
        "Debug Event Handlers/Default/DefaultCreateProcessDebugEventHandler.hpp"
        "Debug Events/DebugEvents.hpp"
        "Abstractions/Debug Event Handlers/AbstractDebugEventHandlers.hpp"
        "Abstractions/Debug Event Handlers/AbstractCreateThreadDebugEventHandler.hpp"
        "Debug Events/CreateThreadDebugEvent.hpp"
        "Debug Event Handlers/Default/DefaultCreateThreadDebugEventHandler.cpp"
        "Debug Event Handlers/Default/DefaultCreateThreadDebugEventHandler.hpp"
        "Abstractions/Debug Event Handlers/AbstractExitProcessDebugEventHandler.hpp"
        "Debug Events/ExitProcessDebugEvent.hpp"
        "Debug Events/ExitThreadDebugEvent.hpp" "Debug Events/UnloadDllDebugEvent.hpp"
        "Abstractions/Debug Event Handlers/AbstractExitThreadDebugEventHandler.hpp"
        "Abstractions/Debug Event Handlers/AbstractUnloadDllDebugEventHandler.hpp"
        "Debug Event Handlers/Default/DefaultExitProcessDebugEventHandler.cpp"
        "Debug Event Handlers/Default/DefaultExitProcessDebugEventHandler.hpp"
        "Debug Event Handlers/Default/DefaultUnloadDllDebugEventHandler.cpp"
        "Debug Event Handlers/Default/DefaultUnloadDllDebugEventHandler.hpp"
        "Debug Event Handlers/Default/DefaultExitThreadDebugEventHandler.cpp"
        "Debug Event Handlers/Default/DefaultExitThreadDebugEventHandler.hpp" "Abstractions/Debug Event Handlers/AbstractExceptionDebugEventHandler.hpp" "Debug Events/ExceptionDebugEvent.hpp" "Debug Event Handlers/Default/DefaultExceptionDebugEventHandler.cpp" "Debug Event Handlers/Default/DefaultExceptionDebugEventHandler.hpp" Debugger/ExceptionContext.hpp Debugger/Breakpoint.hpp "Debug Event Handlers/Default/DefaultBreakpointOnStartHandler.cpp" "Debug Event Handlers/Default/DefaultBreakpointOnStartHandler.hpp" Debugger/Breakpoint.cpp "Debug Event Handlers/TraceResolver.cpp" "Debug Event Handlers/TraceResolver.hpp" "Debug Event Handlers/Dump Function Resolver/DumpFunction.cpp" "Debug Event Handlers/Dump Function Resolver/DumpFunction.hpp" "Debug Event Handlers/Default/DefaultBreakpointResolver.cpp" "Debug Event Handlers/Default/DefaultBreakpointResolver.hpp" "Debug Event Handlers/Dump Function Resolver/ArgInfo.cpp" "Debug Event Handlers/Dump Function Resolver/ArgInfo.hpp" "Debug Event Handlers/Dump Function Resolver/FunctionsDatabase.hpp" "Debug Event Handlers/Dump Function Resolver/StructuresDatabase.hpp" "Debug Event Handlers/Dump Function Resolver/BitfieldsDatabase.hpp" Debugger/Function.cpp Debugger/Function.hpp "Debug Event Handlers/Trace Functions Resolver/TraceFunctionsResolver.hpp" "Debug Event Handlers/Trace Functions Resolver/TraceFunctionsResolver.cpp" "Logger/Message Handlers/FileMessageHandler.hpp")

target_include_directories(Debugger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Debugger PRIVATE Zydis)

find_library(
        DBGHELP_LIB
        NAMES dbghelp.lib
        HINTS "../DBG Kit"
        REQUIRED
)

target_link_libraries(Debugger PRIVATE ${DBGHELP_LIB})
