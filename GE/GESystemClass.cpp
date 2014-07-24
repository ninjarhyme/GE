#include "GESystemClass.h"

#include "GEInputClass.h"

namespace GE
{
	SystemClass* g_systemClass;

	//-----------------------------------------------------------------------
	SystemClass::SystemClass()
	{
	}

	//-----------------------------------------------------------------------
	SystemClass::SystemClass(const SystemClass&)
	{
	}

	//-----------------------------------------------------------------------
	SystemClass::~SystemClass()
	{
	}

	//-----------------------------------------------------------------------
	bool SystemClass::Initialize()
	{
		m_hInstance = GetModuleHandle(NULL);
		InitWindows();
		return true;
	}

	//-----------------------------------------------------------------------
	void SystemClass::Shutdown()
	{
		ShutdownWindows();
	}

	//-----------------------------------------------------------------------
	void SystemClass::InitWindows()
	{
		WNDCLASSEX wc;
		wc.cbSize         = sizeof(WNDCLASSEXW);                         //������Ĵ�С
		wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;         //���ڵķ��
		wc.lpfnWndProc    = WndProc;                                     //ָ��������ָ�룬WndProc�������ڴ���ϵͳ��Ϣ
		wc.cbClsExtra     = 0;                                           //����������ṹ��ĸ����ֽ���
		wc.cbWndExtra     = 0;                                           //��������ʵ����ĸ����ֽ���
		wc.hInstance      = m_hInstance;                                 //��ģ���ʵ�����
		wc.hIcon          = LoadIcon(NULL, IDI_WINLOGO);                 //ͼ��ľ��
		wc.hCursor        = LoadCursor(NULL, IDC_ARROW);                 //���ľ��
		wc.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);         //������ˢ�ľ��
		wc.lpszMenuName   = NULL;                                        //ָ��˵���ָ��
		wc.lpszClassName  = m_applicationName;                           //ָ�������Ƶ�ָ��
		wc.hIconSm        = wc.hIcon;                                    //ָ�򴰿�����Сͼ��ľ��

		RegisterClassEx(&wc);

		//�豸��ʼ��(ȫ�����
		int         posX;               //�������Ͻǵ�λ��X
		int         posY;               //�������Ͻǵ�λ��Y

		if( m_isFullScreen )
		{
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

			dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth  = (unsigned long)m_width;
			dmScreenSettings.dmPelsHeight = (unsigned long)m_height;
			dmScreenSettings.dmBitsPerPel = 32;			
			dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			posX = posY = 0;
		}
		else
		{
			posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;
		}

		//��������
		m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,                  //���ڵ���չ���
			m_applicationName,                                    //ָ�򴰿������ֵ�ָ��
			m_applicationName,                                    //ָ�򴰿����ֵ�ָ��
			WS_OVERLAPPEDWINDOW,                                  //���ڵķ��
			posX, posY,                                           //��������Ļ����ʾ����ʼλ��
			m_width, m_height,                                    //���ڵĿ�Ⱥ͸߶�
			NULL,                                                 //ָ�򱻴������ڵĵĸ����ڵľ��
			NULL,                                                 //�˵��ľ��
			m_hInstance,                                          //�봰���������ģ���ʵ�����
			NULL);                                                //ָ��һ��ֵ��ָ�룬��ֵ���ݸ�����WM_CREATE��Ϣ


		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);


		//������δ�����������������ƶ�����
		POINT lt,rb;
		GetClientRect(m_hWnd,&m_rect);  //ȡ�ô����ڲ�����
		//���������ϵ��������lt��
		lt.x = m_rect.left;
		lt.y = m_rect.top;
		//�����������������rb��
		rb.x = m_rect.right;
		rb.y = m_rect.bottom;
		//��lt��rb�Ĵ�������ת��Ϊ��Ļ����
		ClientToScreen(m_hWnd,&lt);
		ClientToScreen(m_hWnd,&rb);
		//����Ļ���������趨��������
		m_rect.left = lt.x;
		m_rect.top = lt.y;
		m_rect.right = rb.x;
		m_rect.bottom = rb.y;
		//����������ƶ�����
		//ClipCursor(&m_rect);

		m_insideCentre.x = (m_rect.right - m_rect.left) / 2;
		m_insideCentre.y = (m_rect.bottom - m_rect.top) / 2;
		m_outsideCentre.x = m_rect.left + m_insideCentre.x;
		m_outsideCentre.y = m_rect.top + m_insideCentre.y;
		ShowCursor(false);		//���������

		return;
	}

	//-----------------------------------------------------------------------
	void SystemClass::ShutdownWindows()
	{
		//ShowCursor(true);     //��ʾ���

		if( m_isFullScreen )
		{
			ChangeDisplaySettings(NULL, 0);
		}

		DestroyWindow(m_hWnd);
		m_hWnd = NULL;

		UnregisterClass(m_applicationName, m_hInstance);
		m_hInstance = NULL;

		return;
	}

	//-----------------------------------------------------------------------
	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch(umessage)
		{
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
		case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}
		default:
			{
				return g_inputClass->MessageHandler(hwnd, umessage, wparam, lparam);
			}
		}
	}
}