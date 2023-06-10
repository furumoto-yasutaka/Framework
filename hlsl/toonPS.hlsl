#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�s�N�Z���̖@���𐳋K��
	float4 normal = normalize(In.Normal);
	//�����v�Z������
	float light = -dot(normal.xyz, Light.Direction.xyz);
    	// ���邳������
    if (light <= 0.001)
    {
        light = 0.6;
    }
    else if (light <= 0.3)
    {
        light = 0.8;
    }
    else
    {
        light = 1.0;
    }
    
    //�J��������s�N�Z���֌������x�N�g��
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev); //���K������
    
    float d = dot(normal.xyz, eyev);
    
    //�e�N�X�`���̃s�N�Z���F���擾
    outDiffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *=
		In.Diffuse.rgb * light; //���邳�ƐF����Z
    outDiffuse.a *=
		In.Diffuse.a; //���ʌv�Z
    
    if (d >= -0.3)
    {
        outDiffuse.rgb = 0;
    }
}


