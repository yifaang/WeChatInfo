#pragma <waring:disable>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
using namespace std;

DWORD GetProcPid(const char* Processname) {
    // From Process name Get ProcessId
    PROCESSENTRY32  info;
    info.dwSize = sizeof(PROCESSENTRY32);
    bool Found;
    HANDLE ProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (ProcessSnap == NULL) {
        cout << "Create Snap Failed" << endl;
        return -1;
    }
    bool pSnap = Process32First(ProcessSnap, &info);
    while (pSnap)
    {
        if (_strcmpi((const char*)info.szExeFile, (const char*)Processname) == 0) {
            return info.th32ProcessID;
        }
        pSnap = Process32Next(ProcessSnap, &info);
        //printf("Processname:%s\n",info.szExeFile);
    }
    CloseHandle(ProcessSnap);
    return 0;
}

DWORD GetFatherPid() {
    //Get Process Father Process Id
    PROCESSENTRY32 info;
    info.dwSize = sizeof(PROCESSENTRY32);
    bool Found;
    HANDLE ProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (ProcessSnap == NULL) {
        cout << "Create Snap Failed" << endl;
        return -1;
    }
    bool pSnap = Process32First(ProcessSnap, &info);
    while (pSnap)
    {
        if (info.th32ProcessID == GetCurrentProcessId()) {
            return info.th32ParentProcessID;
        }
        pSnap = Process32Next(ProcessSnap, &info);
    }
    CloseHandle(ProcessSnap);
}




void Anti_Cloudware() {
    //Anti CloudVirustual 
    SYSTEM_INFO systeminfo;
    GetSystemInfo(&systeminfo);
    int CoreNum = systeminfo.dwNumberOfProcessors;
    if (CoreNum >= 4)
    {
        cout << "Is Not running in Cloud Ware" << endl;
    }
    else {
        cout << "Is running in Cloud Ware! " << endl;
        exit(-1);
    }
}


void Anti_Debug() {
    //Anti DeBuger
    DWORD ExplorerId = GetProcPid("explorer.exe");
    DWORD FatherId = GetFatherPid();
    if (ExplorerId == FatherId) {
        cout << "Process is not Running with Debuger" << endl;
    }
    else {
        cout << "Process Running with Debuger \nProcess Exit" << endl;
        exit(-1);
    }
}

void Anti_Vmware() {
    //Anti Virtual Machine
    DWORD IsVmware = GetProcPid("vmtoolsd.exe");
    if (IsVmware == 0) {
        cout << "Is Not running in vmware" << endl;
    }
    else {
        cout << "Is running in vmware " << endl;
        exit(-1);
    }
}