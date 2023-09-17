#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace dx9
{
	typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

	bool GetD3D9Device(void** pTable, size_t size);
	HWND GetProcessWindow();

	static HWND window;
	extern LONG windowHeight, windowWidth;
}