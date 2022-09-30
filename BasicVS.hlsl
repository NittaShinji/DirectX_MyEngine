#include"Basic.hlsli"

//P04_01_P25
VSOutput main(float4 pos : POSITION, float3 normal : NORMAL,float2 uv : TEXCOORD,float4 rgba : RGBA)
{
	VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
	//output.svpos = pos;
	 
	//�V�X�e���p���_���W
	output.svpos = mul(mat, pos);//���W��3D�ϊ��s�����Z
	//UV�l
	output.uv = uv;
	//�@���x�N�g��
	output.normal = normal;
	//�J���[
	output.rgba = rgba;

	return output;
}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos + float4( 0,0,1,1);
//}