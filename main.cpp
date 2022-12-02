import Debugger;
import Logger;
import ConsoleMessageHandler;

int main()
{
    Logger::GetInstance()->AddMessageHandler(new ConsoleMessageHandler());
    Debugger* debugger = Debugger::GetInstance();

    debugger->StartAndAttach(R"(C:\Windows\System32\notepad.exe)");

    return 0;
}
