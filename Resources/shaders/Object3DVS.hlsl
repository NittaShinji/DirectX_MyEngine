#include"Object3D.hlsli"

//P04_01_P25
//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{
//    VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
//	//output.svpos = pos;
	 
//	//�V�X�e���p���_���W
//    output.svpos = mul(mat, pos); //���W��3D�ϊ��s�����Z
//	//UV�l
//    output.uv = uv;
//    output.normal = normal;
//    return output;
//}

//float4 main() : SV_TARGET
//{
//	return float4(1.0f,1.0f,1.0f,1.0f);
//}

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL,float2 uv : TEXCOORD)
{
 //   //�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
 //   float4 wnormal = normalize(mul(world, float4(normal, 0)));
 //   float4 wpos = mul(world, pos);
	
	////�����ˌ�
 //   float3 ambient = m_ambient;
	////�g�U���ˌ�
 //   float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
	////���_���W
 //   float3 eyeDir = normalize(cameraPos - wpos.xyz);
	
	////����x
 //   const float shininess = 4.0f;
	////���ˌ��x�N�g��
 //   float3 reflect = normalize(-lightv + 2 * dot(-lightv, wnormal.xyz) * wnormal.xyz);
	////���ʔ��ˌ�
 //   float3 specuar = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;

 //   VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
 //   output.svpos = mul(mul(viewproj, world), pos);
 //   output.color.rgb = (ambient + diffuse + specuar) * lightcolor;
 //   output.color.a = m_alpha;
 //   output.uv = uv;
 //   return output;
    
    VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
    
    //matrix mat = world * viewproj;
    //������mat���g���Ă���
    output.svpos = mul(mul(viewproj, world), pos);
    output.normal = normal;
    output.uv = uv;
    return output;
    
}
    //return pos + float4(0, 0, 1, 1);
    
//}

//float4 main(float4 pos : POSITION) : SV_POSITION
//{
    
//    return mul(mat, pos);
//    //VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
//    //output.svpos = mul(mat, pos);
//    //output.normal = normal;
//    //output.uv = uv;
//    //return output;
    
//    //return pos + float4(0, 0, 1, 1);
    
//}