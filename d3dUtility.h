#pragma once
#include <windows.h>
#include <iostream>
#include "d3dx9.h"
namespace d3d
{
	
	bool initD3D(HINSTANCE hInstance, int width, int height, bool isWindow, D3DDEVTYPE deviceType, IDirect3DDevice9** device);

	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	LRESULT CALLBACK winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
}

