#pragma once

#include <d3d11.h>
#include <d3dx11.h>

#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"d3dx11.lib")

namespace Memory
{
	template<typename T>
	void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = NULL;
		}
	}

	template<typename T>
	void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = NULL;
		}
	}

	template<typename T>
	void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = NULL;
		}
	}
}