//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_DEBUGEVENTHANDLERTYPE_HPP
#define DEBUGGER_DEBUGEVENTHANDLERTYPE_HPP


enum DebugEventHandlerType
{
    eCreateProcess,
    eLoadDll,
    eExitProcess,
    eExitThread,
    eException,
    eCreateThread,
    eUnloadDll,
    eOutputDebugString,
    eRIP
};


#endif //DEBUGGER_DEBUGEVENTHANDLERTYPE_HPP
