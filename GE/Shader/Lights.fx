#ifndef _LIGHTS_FX_
#define _LIGHTS_FX_

//ƽ�й���ռ���
void ComputeDirLight(Material mat,			//����
					DirLight dirLight,		//ƽ�й�
					float3 normal,			//���㷨��
					float3 toEye,			//"����->��"����
					out float4 ambient,		//�������������ⲿ��
					out float4 diffuse,		//�������������䲿��
					out float4 specular)	//���������߹ⲿ��
{
	//�����������
	ambient  = float4(0.0f,0.0f,0.f,0.f);
	diffuse  = float4(0.f,0.f,0.f,0.f);
	specular = float4(0.f,0.f,0.f,0.f);

	//������ֱ�Ӽ���
	ambient = mat.ambient * dirLight.ambient;

	//����������ϵ��
	//ע�⣺����ǰ��֤���ߡ����߷����һ��
	float diffFactor = -dot(normal,dirLight.dir);

	//���ϵ��С��0�������㱳�Ź�Դ�������ٽ��м���
	[flatten]
	if(diffFactor > 0)
	{
		//�����������
		diffuse = mat.diffuse * dirLight.diffuse * diffFactor;

		float3 refLight = reflect(dirLight.dir,normal);
		float specFactor = pow(max(dot(refLight,toEye),0.f),mat.specular.w);
		specular = mat.specular * dirLight.specular * specFactor;
	}
}

//���Դ���ռ���
void ComputePointLight(Material mat,			//����
						PointLight pLight,		//���Դ
						float3 normal,			//����
						float3 position,		//����λ��
						float3 toEye,			//"����->��"����
						out float4 ambient,		//�������������ⲿ��
						out float4 diffuse,		//�������������䲿��
						out float4 specular)	//���������߹ⲿ��
{
	//�����������
	ambient  = float4(0.f,0.f,0.f,0.f);
	diffuse	 = float4(0.f,0.f,0.f,0.f);
	specular = float4(0.f,0.f,0.f,0.f);

	//������շ��򣺶���->��Դ
	float3 dir = pLight.pos - position;

	//���㶥�㵽��Դ����
	float dist = length(dir);

	//�������䷶Χ�����ٽ��м���
	if(dist > pLight.range)
		return;

	//��һ�����߷���
	dir /= dist;

	//�����ǿ��˥��
	float att = 1/(pLight.att.x + pLight.att.y*dist + pLight.att.z*dist*dist);

	//���㻷����
	ambient = mat.ambient * pLight.ambient * att;

	//����������ϵ��
	float diffFactor = dot(dir,normal);

	//���С��0��ֱ���˳�
	if(diffFactor > 0)
	{
		//�����������
		diffuse = mat.diffuse * pLight.diffuse * diffFactor * att;

		float3 refLight = reflect(-dir,normal);
		//����߹�ϵ��
		float specFactor = pow(max(dot(refLight,toEye),0.f),mat.specular.w);

		//����߹�
		specular = mat.specular * pLight.specular * specFactor * att;
	}
}

//�۹�ƹ��ռ���
void ComputeSpotLight(Material mat,			//����
					SpotLight L,			//�۹��
					float3 normal,			//����
					float3 position,		//����λ��
					float3 toEye,			//"����->��"����
					out float4 ambient,		//�������������ⲿ��
					out float4 diffuse,		//�������������䲿��
					out float4 specular)	//���������߹ⲿ��
{
	//�����������
	ambient  = float4(0.f,0.f,0.f,0.f);
	diffuse  = float4(0.f,0.f,0.f,0.f);
	specular = float4(0.f,0.f,0.f,0.f);

	//������շ��򣺶���->��Դ
	float3 dir = L.pos - position;
	//���㶥�㵽��Դ����
	float dist = length(dir);

	//���������ڹ��շ�Χ�����ٽ��м���
	if(dist > L.range)
		return;

	//��һ�����߷���
	dir /= dist;
	
	//����˥��ϵ��
	float att = 1/(L.att.x + L.att.y*dist + L.att.z*dist*dist);

	//����۹�˥��ϵ��
	float tmp = -dot(dir,L.dir);
	if(tmp < cos(L.theta))
		return;
	float spotFactor = pow(max(tmp,0.f),L.spot);

	//���㻷����
	ambient = mat.ambient * L.ambient * att * spotFactor;

	//����������ϵ��
	float diffFactor = dot(dir,normal);

	//���С��0��ֱ���˳�
	if(diffFactor > 0)
	{
		//�����������
		diffuse = mat.diffuse * L.diffuse * diffFactor * att * spotFactor;
		
		float3 refLight = reflect(-dir,normal);
		//����߹�ϵ��
		float specFactor = pow(max(dot(refLight,toEye),0.f),mat.specular.w);

		//����߹�
		specular = mat.specular * L.specular * specFactor * att * spotFactor;
	}
}

#endif