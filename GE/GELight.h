#pragma once
#ifndef _GELIGHT_H_
#define _GELIGHT_H_

#include "GEMath.h"

namespace GE
{
	//ƽ�й�
	struct DirLight
	{
		FLOAT4	ambient;	//������
		FLOAT4	diffuse;	//�������
		FLOAT4	specular;	//�߹�

		FLOAT3	dir;		//���շ���
		FLOAT	unused;		//������HLSL��"4D����"�������ƥ��
	};

	//���Դ
	struct PointLight
	{
		FLOAT4	ambient;	//������
		FLOAT4	diffuse;	//�������
		FLOAT4	specular;	//�߹�

		FLOAT3	pos;		//��Դλ��
		FLOAT	range;		//���շ�Χ

		FLOAT3	att;		//˥��ϵ��
		FLOAT	unused;		//������HLSL��"4D����"�������ƥ��
	};

	//�۹��
	struct SpotLight
	{
		FLOAT4	ambient;	//������
		FLOAT4	diffuse;	//�������
		FLOAT4	specular;	//�߹�

		FLOAT3	dir;		//���շ���
		FLOAT	range;		//���շ�Χ

		FLOAT3	pos;		//��Դλ��
		FLOAT	spot;		//�۹�ǿ��ϵ��

		FLOAT3	att;		//˥��ϵ��
		FLOAT	theta;		//���ɢ�Ƕ�
	};

	//����
	struct Material
	{
		FLOAT4	ambient;
		FLOAT4	diffuse;
		FLOAT4	specular;	//��4��Ԫ��Ϊ���ʵľ��淴��ϵ������������ʱ���Ĺ⻬�̶�
	};
};

#endif