#pragma once
#include <Windows.h>

#include "../SDK/Utils/Utils.h"

bool detour_hook(void* src, void* dst, int len);
BYTE* trampoline_hook(BYTE* src, BYTE* dst, const DWORD len);

class Hook
{
public:
	bool toggle_status = false;

	BYTE* src = nullptr;
	BYTE* dst = nullptr;
	BYTE* p_gateway = nullptr;
	int len = 0;

	BYTE originalBytes[10]{ 0 };

	Hook(BYTE* src, BYTE* dst, BYTE* ptrToGatewayPtr, uintptr_t len);
	Hook(const char* exportName, const char* moduleName, BYTE* dst, BYTE* ptrToGatewayPtr, uintptr_t len);

	void enable();
	void disable();
	void toggle();
};