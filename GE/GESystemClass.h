#pragma once
#ifndef _GESYSTEMCLASS_H_
#define _GESYSTEMCLASS_H_

#include <windows.h>
#include "GEManageClass.h"

//-----------------------------------------------------------------------
namespace GE
{
	class SystemClass : public GEClass
	{
	public:
		SystemClass();
		SystemClass(const SystemClass&);
		~SystemClass();  

		bool Initialize();
		void Shutdown();

	private:  
		void InitWindows();           //��������
		void ShutdownWindows();       //���ٴ���
	};

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	extern SystemClass* g_systemClass;
}

#endif