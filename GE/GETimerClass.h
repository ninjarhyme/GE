#pragma once
#ifndef _GETIMERCLASS_H_
#define _GETIMERCLASS_H_

#include <windows.h>
#include "GEManageClass.h"

//-----------------------------------------------------------------------
namespace GE
{
	class TimerClass : public GEClass
	{
	public:
		TimerClass();
		TimerClass(const TimerClass&);
		~TimerClass();

		bool Initialize();
		void Shutdown();

		void Tick();				//��ʱһ�Σ�ÿһ֡����һ��

		double TotalTime() const;	//���ض�ʱ���ӿ�ʼ��ʱ�����ڵ���ʱ�䣨��������ͣʱ�䣩����λ����
		double DeltaTime() const;	//���ر��μ�ʱ���ϴμ�ʱ֮���ʱ����λ����

		void Start();				//��ͣ��ָ���ʱ
		void Stop();				//��ͣ
		void Reset();				//���ã���ʼ��ʱ����ʱ����ʼ����ʱ����

		void ShowFPSInTitle();
		void ShowFPSInTitle(LPCWSTR _title);
	private:
		double	m_secsPerCount;		//ϵͳ�йأ�����ϵͳ������֮���ʱ���ֵ����λ����
		//��������Ƕ�ʱ����ʱ�䵥λת��Ϊ���롱�Ļ�׼
		double	m_deltaTime;		//�����ϴμ�ʱ֮���ʱ����λ����

		__int64 m_baseTime;			//��ʱ����ʼ��������ʼʱ���
		__int64 m_stopTime;			//�����ǰΪ��ͣ״̬����ָ��ʱ����ʼʱ��
		__int64 m_pausedTime;		//��ͣ����ʱ���ۼ�
		__int64	m_currTime;			//���μ�ʱ��ʱ���
		__int64	m_prevTime;			//�ϴμ�ʱ��ʱ���

		bool	m_stopped;			//�Ƿ���ͣ
	};

	extern TimerClass* g_timerClass;

	void GEShowFPSInTitle();
	void GEShowFPSInTitle(LPCWSTR _title);
	double GEDeltaTime();
}

#endif