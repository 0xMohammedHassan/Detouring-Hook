#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>


BOOL Hook(void* HookAddr, void* Function , int len) 
{
	DWORD oldProt;
	if (len >= 5) {

		if (!VirtualProtect(HookAddr, len, PAGE_EXECUTE_READWRITE, &oldProt)) {

			MessageBoxA(0, "Cant Change memory page protection1! -> ", 0, 0);

			
		}
		//Now we can set the NOP sleds for safety
		memset(HookAddr, 0x90, len);

		//char buff[100];

		DWORD relativeAddr = ((DWORD)Function - (DWORD)HookAddr) - 5;// calculate the size of the jump

		*(BYTE*)HookAddr = 0xE9; // set the first byte to E9 what is the jump code

		 *(DWORD*)((DWORD)HookAddr + 1)= relativeAddr; // add 1 to the address (so you don't overwrite the E9) and set the size of the jump

		/* sprintf(buff, "RelativeAddr %x", relativeAddr);
		 
		 MessageBoxA(0, buff, "Address", 0);*/

		DWORD OriginalProt;
		if (!VirtualProtect(HookAddr, len, oldProt, &OriginalProt)) {

			MessageBoxA(0,"Cant Change memory page protection2! -> ",0,0);
			
		}

		return TRUE;

	}
	else {

		return FALSE;

	}

}

DWORD JumpBack;

__declspec(naked) void ShellCode() {
	__asm {
		/*
		* TIP :
		
		Most of the time if you're messing with detours you're not restoring the code back or something so it's jumping to a module that doesn't exist.
		*/
			/*
			Original bytes

00271EEF - 83 E8 05 - sub eax,05
00271EF2 - 50 - push eax
00271EF3 - A3 783A2A00 - mov [testers7.health],eax <<
			
			*/
		//sub eax,1 // Patching eax
		//push eax // same like original bytes
		//mov  dword ptr   ds: [0x2a3a78] , eax // i need to diassemble these bytes A3 783A2A00 because in our shellcode we are overwriting the whole bytes so we will need to replace them all with something like the previous one , also mov ds:0x2a3a78,eax works fine 
	

		add[eax + 0x00005578],0x50 // that mean adding 80 in decimal, this is for PlantsAndZombie game :)
		jmp [JumpBack] // jmp to continue the execution flow of the demo software
		
	}
}
DWORD WINAPI MainThread(LPVOID param) {
	//MessageBoxA(0, "Attached !", "Done", 0);

	int hookLength = 6; //we are hooking these bytes 83 E8 05 50 A3 783A2A00 , since we cant hook only 5 because there will be a rest bytes must be used or the demo will be crashed , i hope someone tell me what is the issue

	DWORD hookAdreess = 0x41E6E0; //0x271EEF;
	JumpBack = hookAdreess + hookLength;

	if (!Hook((void*)hookAdreess, ShellCode, hookLength)) {

		MessageBoxA(0, "Something is wrong !", "wrong", 0);
	}

	while (TRUE) {

		if (GetAsyncKeyState(VK_F1)) {
			MessageBoxA(0, "Deattached from the process !", "Bye", 0);
			break;
		}
		Sleep(100);
	}


	FreeLibraryAndExitThread((HMODULE)param, 0);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) 
{

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0,0);
		break;

	}
	return TRUE;
}

