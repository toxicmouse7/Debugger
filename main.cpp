//#include "Debugger/Debugger.hpp"
//#include "Debug Event Handlers/Default/DefaultHandlers.hpp"
//#include "Logger/Logger.hpp"
//#include "Logger/Message Handlers/MessageHandlers.hpp"
//#include "Debug Event Handlers/TraceDebugEventHandler.hpp"
#include <dia2.h>
#include <atlbase.h>
#include <iostream>

HRESULT CoCreateDiaDataSource(CComPtr<IDiaDataSource>& data_source)
{
    HMODULE hmodule = LoadLibraryA("msdia140");
    if (!hmodule)
        return HRESULT_FROM_WIN32(GetLastError()); // library not found

    auto DllGetClassObject = (BOOL(WINAPI*)(REFCLSID, REFIID, LPVOID*)) GetProcAddress(hmodule, "DllGetClassObject");

    if (!DllGetClassObject)
        return HRESULT_FROM_WIN32(GetLastError());

    CComPtr<IClassFactory> pClassFactory;
    HRESULT hr = DllGetClassObject(CLSID_DiaSource, IID_IClassFactory, (void**) &pClassFactory);
    if (FAILED(hr))
        return hr;

    hr = pClassFactory->CreateInstance(NULL, IID_IDiaDataSource, (void**) &data_source);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

int main()
{
    CoInitialize(nullptr);
    CComPtr<IDiaDataSource> pSource;
    auto hr = CoCreateDiaDataSource(pSource);
    if (FAILED(hr))
    {
        std::cout << "Code: " << HRESULT_CODE(hr);
        std::cout << "Could not CoCreate CLSID_DiaSource. Register msdia*.dll." << std::endl;
        return -1;
    }
    wchar_t wszFilename[MAX_PATH];
    mbstowcs_s(nullptr, wszFilename, R"(C:\Users\Aleksej\CLionProjects\test123\cmake-build-debug-x64\test.pdb)",
               MAX_PATH);
    if (FAILED(pSource->loadDataFromPdb(wszFilename)))
    {
        return -1;
    }

    CComPtr<IDiaSession> pSession;
    if (FAILED(pSource->openSession(&pSession)))
    {
        return -1;
    }

    CComPtr<IDiaSymbol> pGlobal;
    if (FAILED(pSession->get_globalScope(&pGlobal)))
    {
        return -1;
    }

    CComPtr<IDiaEnumSymbols> pEnumSymbols;
    if (FAILED(pGlobal->findChildren(SymTagCompiland, nullptr, nsNone, &pEnumSymbols)))
    {
        return -1;
    }

    CComPtr<IDiaSymbol> pCompiland;
    ULONG celt = 0;

    while (SUCCEEDED(pEnumSymbols->Next(1, &pCompiland, &celt)) && celt == 1)
    {
        std::wcout << "\n** Module: ";
        BSTR bstrName;
        if (pCompiland->get_name(&bstrName) != S_OK)
        {
            std::wcout << L"(???)\n\n";
        }
        else
        {
            std::wcout << bstrName << "\n\n";
            SysFreeString(bstrName);
        }

        CComPtr<IDiaEnumSymbols> pEnumChildren;
        if (SUCCEEDED(pCompiland->findChildren(SymTagNull, nullptr, nsNone, &pEnumChildren)))
        {
            CComPtr<IDiaSymbol> pSymbol;
            ULONG celtChildren = 0;
            while (SUCCEEDED(pEnumChildren->Next(1, &pSymbol, &celtChildren)) && celtChildren == 1)
            {
                if (pSymbol->get_name(&bstrName) == S_OK)
                {
                    std::wcout << bstrName << '\n';
                    SysFreeString(bstrName);
                }
                pSymbol.Release();
            }

            pEnumChildren.Release();
        }

        pCompiland.Release();
    }
//    Debugger* debugger = Debugger::GetInstance();
//    auto& logger = Logger::GetInstance("Debugger");
//
//    logger.AddMessageHandler(new ConsoleMessageHandler());
//
//    debugger->AddLoadDllHandler<DefaultLoadDllDebugEventHandler>(logger);
//    debugger->AddCreateProcessHandler<DefaultCreateProcessDebugEventHandler>(logger);
//    debugger->AddUnloadDllHandler<DefaultUnloadDllDebugEventHandler>(logger);
//    debugger->AddExitProcessHandler<DefaultExitProcessDebugEventHandler>(logger);
//    debugger->AddExitThreadHandler<DefaultExitThreadDebugEventHandler>(logger);
//    debugger->AddCreateThreadHandler<DefaultCreateThreadDebugEventHandler>(logger);
//
//    debugger->SetErrorLogger(logger);
//
////    auto& debuggerState = debugger->CreateAndAttach(R"(C:\Windows\System32\notepad.exe)", false);
//    auto& debuggerState = debugger->CreateAndAttach(
//            R"(C:\Users\Aleksej\CLionProjects\test123\cmake-build-release\test.exe)", true);
//
//    while (debuggerState != DebuggerState::Stopped)
//    {
//        auto exceptionContext = debugger->WaitForDebugEvent();
//
//        if (exceptionContext.IsExternal() && exceptionContext.Type() != ExceptionContextType::eHardBreakpoint)
//        {
//            debugger->UseResolver<DefaultExceptionDebugEventHandler>(exceptionContext, logger);
//        }
//        else
//        {
//            debugger->UseResolver<TraceDebugEventHandler>(exceptionContext, logger);
//        }
//    }
    system("PAUSE");
    return 0;
}
