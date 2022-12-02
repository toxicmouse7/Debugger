cmake_minimum_required(VERSION 3.23)
project(Debugger)

set(CMAKE_CXX_STANDARD 20)

add_executable(Debugger main.cpp Debugger.cpp Debugger.hpp "Debug Events/DebugEventType.hpp" "Debug Events/DebugEvent.hpp" "Debug Events/DebugEvent.cpp" "Debug Event Hadlers/Default/DefaultCreateProcessDebugEventHandler.hpp" "Abstractions/Message Handling/AbstractMessageHandler.hpp" "Logger/Message Handlers/ConsoleMessageHandler.hpp" Logger/Logger.cpp Logger/Logger.hpp "Debug Event Hadlers/Default/DefaultExceptionDebugEventHandler.hpp" "Debug Event Hadlers/Default/DefaultLoadDllDebugEventHandler.hpp" "Process Memory Manipultion/ProcessMemoryManipulation.cpp" "Process Memory Manipultion/ProcessMemoryManipulation.hpp" "Abstractions/Event Managing/AbstractEvent.hpp" "Abstractions/Event Managing/AbstractEventManager.hpp" "Abstractions/Event Managing/AbstractEventRecipient.hpp" Utils/Utils.cpp Utils/Utils.hpp "Debug Event Hadlers/Default/DefaultLoadDllDebugEventHandler.cpp" "Debug Event Hadlers/Default/DefaultCreateThreadDebugEventHandler.cpp" "Debug Event Hadlers/Default/DefaultCreateThreadDebugEventHandler.hpp" "Debug Event Hadlers/Default/DefaultCreateProcessDebugEventHandler.cpp")

target_include_directories(Debugger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

