//
// No x64 support yet
//
#include "Hooks.h"

// Detour hook for 32 bit apps/games
bool detour_hook(void* src, void* dst, int len)
{
	// Length must be atleast 5 bytes
	if (len < 5)
		// If its less return false, dont hook
		return false;

	// Current protection
	DWORD curProtection;

	// Get read & write permissions
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	// Nop toHook instructions
	memset(src, 0x90, len);

	// Get the relative address (offset from the jmp to where we need to go)
	DWORD relativeAddress = ((DWORD)dst - (DWORD)src) - 5;

	// Place the jmp instruction. 0xE9 is first opcode to the jump instruction
	*(BYTE*)src = 0xE9;

	// Convert it to DWORD ptr (add 1 so we are not overwritting the jmp (0xE9))
	*(DWORD*)((DWORD)src + 1) = relativeAddress;

	// Temporary variable
	DWORD temp;

	// Restore read & write permissions
	VirtualProtect(src, len, curProtection, &temp);

	// Return true
	return true;
}

// Trampoline hook 32 bit
BYTE* trampoline_hook(BYTE* src, BYTE* dst, const DWORD len)
{
	// Length must be atleast 5 bytes
	if (len < 5)
		// If its less return 0, dont hook
		return 0;

	// Create the gateway
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	// Write the stolen bytes to the getway
	memcpy_s(gateway, len, src, len);

	// Get the gateway to the destination address (relative address)
	DWORD gatewayRelativeAddress = src - gateway - 5;

	// Add jmp opcode to the end of the gateway
	*(gateway + len) = 0xE9;

	// Write the address of the gateway to the jmp
	*(DWORD*)((DWORD)gateway + len + 1) = gatewayRelativeAddress;

	// Perform the detour
	detour_hook(src, dst, len);

	// Return the gateway
	return gateway;
}

Hook::Hook(BYTE* src, BYTE* dst, BYTE* ptrToGatewayPtr, uintptr_t len)
{
	this->src = src;
	this->dst = dst;
	this->len = len;
	this->p_gateway = ptrToGatewayPtr;
}

Hook::Hook(const char* exportName, const char* moduleName, BYTE* dst, BYTE* ptrToGatewayPtr, uintptr_t len)
{
	HMODULE hMod = GetModuleHandleA(moduleName);

	this->src = (BYTE*)GetProcAddress(hMod, exportName);
	this->dst = dst;
	this->len = len;
	this->p_gateway = ptrToGatewayPtr;
}

void Hook::enable()
{
	memcpy(originalBytes, src, len);
	*(uintptr_t*)p_gateway = (uintptr_t)trampoline_hook(src, dst, len);
	toggle_status = true;
}

void Hook::disable()
{
	Utils::patch(src, originalBytes, len);
	toggle_status = false;
}

void Hook::toggle()
{
	if (!toggle_status)
		enable();
	else
		disable();
}