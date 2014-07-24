#include "GETimerClass.h"

#include <stdio.h>

namespace GE
{
	TimerClass* g_timerClass;

	//-----------------------------------------------------------------------
	TimerClass::TimerClass()
	{
	}

	//-----------------------------------------------------------------------
	TimerClass::TimerClass(const TimerClass& other)
	{
	}

	//-----------------------------------------------------------------------
	TimerClass::~TimerClass()
	{
		Shutdown();
	}

	//-----------------------------------------------------------------------
	bool TimerClass::Initialize()
	{
		__int64 frequency;
		//�ռ�ϵͳ��Ϣ����ʼ��������֮���ʱ���
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
		m_secsPerCount = 1.f/frequency;

		m_deltaTime = 0.f;

		m_baseTime    = 0;
		m_stopTime    = 0;
		m_pausedTime  = 0;
		m_currTime    = 0;
		m_prevTime    = 0;

		m_stopped = false;

		return true;
	}

	//-----------------------------------------------------------------------
	void TimerClass::Shutdown()
	{
	}

	//-----------------------------------------------------------------------
	double TimerClass::TotalTime() const
	{
		//�������ͣ״̬������ͼ��ʾ��
		//	��ʼʱ��T0			��ͣ��ʼʱ��S	��ǰʱ��T
		//		|---------------------|------------|
		//�ܵ�ʱ��Ϊ:S - T0 - ��ͣ���ۼ�ʱ��
		//ע��Ѽ���ת��Ϊ��
		if(m_stopped)
		{
			return (m_stopTime-m_baseTime-m_pausedTime)*m_secsPerCount;
		}
		//���������״̬
		//	��ʼʱ��T0							��ǰʱ��T
		//		|----------------------------------|
		//�ܵ�ʱ��Ϊ��T - T0 - ��ͣ���ۼ�ʱ��
		//ע��Ѽ���ת��Ϊ��
		else
		{
			return (m_currTime-m_baseTime-m_pausedTime)*m_secsPerCount;
		}
	}

	//-----------------------------------------------------------------------
	double TimerClass::DeltaTime() const
	{
		//�������ϴμ�ʱ�м��ʱ���
		return m_deltaTime;
	}

	//-----------------------------------------------------------------------
	void TimerClass::Start()
	{
		//��ͣ�ָ�
		//�����������ͣ״̬���򲻲���
		//���Ϊ��ͣ״̬
		if(m_stopped)
		{
			//�Ȼ�ȡ��ʼʱ��
			__int64 startTime;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

			//�ѱ�����ͣʱ���ۼƵ�����ͣʱ����
			m_pausedTime += (startTime-m_stopTime);
			m_prevTime = startTime;
			m_stopTime = 0;
			m_stopped = false;
		}
	}

	//-----------------------------------------------------------------------
	void TimerClass::Stop()
	{
		if(!m_stopped)
		{
			__int64 curr;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));
			m_stopTime = curr;
			m_stopped = true;
		}
	}

	//-----------------------------------------------------------------------
	void TimerClass::Reset()
	{
		//���ö�ʱ�������빤��״̬
		__int64 curr;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));
		//ȷ����ʼʱ��
		m_baseTime = curr;
		m_currTime = curr;
		m_prevTime = curr;
		m_deltaTime = 0.f;

		m_stopTime = 0;
		//��ͣ�ۼ�����
		m_pausedTime = 0;
		m_stopped = false;
	}

	//-----------------------------------------------------------------------
	void TimerClass::Tick()
	{
		//ÿ֡���õļ�ʱ����
		//���ڸ��¶�ʱ��״̬

		//���Ϊ��ͣ״̬���򲻲���
		if(m_stopped)
		{
			m_deltaTime = 0.f;
			return;
		}

		//�Ȼ�ȡ��ǰʱ��
		__int64 curr;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));
		m_currTime = curr;
		//���������һ�μ�ʱ��ʱ���
		m_deltaTime = (m_currTime-m_prevTime)*m_secsPerCount;
		//ȷ��Ϊ����(��ʵһ������²�����Ϊ��)
		m_deltaTime = m_deltaTime<0? 0.f : m_deltaTime;

		//���ϴ�ʱ�����Ϊ����ʱ�䣬���´�Tick��
		m_prevTime = m_currTime;
	}

	//-----------------------------------------------------------------------
	void TimerClass::ShowFPSInTitle()
	{
		static float begin = (float)TotalTime();
		static int frameCounter = 0;
		if((float)TotalTime() - begin >= 1.f)
		{
			wchar_t text[50];
			swprintf_s(text,L"%s%d ",L"FPS:",frameCounter);
			SetWindowText(m_hWnd, text);
			begin = (float)TotalTime();
			frameCounter = 0;
		}
		++frameCounter;
	}

	//-----------------------------------------------------------------------
	void TimerClass::ShowFPSInTitle(LPCWSTR _title)
	{
		static float begin = (float)TotalTime();
		static int frameCounter = 0;
		if((float)TotalTime() - begin >= 1.f)
		{
			wchar_t text[50];
			swprintf_s(text,L"%s   %s%d ",_title,L"FPS:",frameCounter);
			SetWindowText(m_hWnd, text);
			begin = (float)TotalTime();
			frameCounter = 0;
		}
		++frameCounter;
	}

	//-----------------------------------------------------------------------
	void GEShowFPSInTitle() { g_timerClass->ShowFPSInTitle(); }

	//-----------------------------------------------------------------------
	void GEShowFPSInTitle(LPCWSTR _title) { g_timerClass->ShowFPSInTitle(_title); }

	//-----------------------------------------------------------------------
	double GEDeltaTime() { return g_timerClass->DeltaTime(); }

	//-----------------------------------------------------------------------
}