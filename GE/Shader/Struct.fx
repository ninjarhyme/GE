#ifndef _STRUCT_FX_
#define _STRUCT_FX_

//ƽ�й�
struct DirLight
{
	float4	ambient;	//������
	float4	diffuse;	//������
	float4	specular;	//�߹�

	float3	dir;		//����
	float	unused;		//��4D������������
};

//���Դ
struct PointLight
{
	float4	ambient;	//������
	float4	diffuse;	//�������
	float4	specular;	//�߹�

	float3	pos;		//��Դλ��
	float	range;		//��Դ���䷶Χ

	float3	att;		//��ǿ˥��ϵ��
	float	unused;		//"4D����"������
};

//�۹��
struct SpotLight
{
	float4	ambient;	//������
	float4	diffuse;	//�������
	float4	specular;	//�߹�

	float3	dir;		//����
	float	range;		//���䷶Χ

	float3	pos;		//λ��
	float	spot;		//�۹�ǿ��ϵ��

	float3	att;		//���ϵ��
	float	theta;		//���ɢ�Ƕ�
};

//����
struct Material
{
	float4	ambient;
	float4	diffuse;
	float4	specular;	//specular�е�4��Ԫ�ش�����ʵı���⻬�̶�
};

#endif