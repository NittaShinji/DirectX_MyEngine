#include"TrailHF.hlsli"

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

//PS_IN main(VS_IN input)
//{
//    PS_IN result = (PS_IN) 0;
//    result.pos = mul(g_xCamera_VP, float4(pos, 1));
//    result.uv = uv;

//    return result;
//}

PS_IN main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    ////�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    //float4 wnormal = normalize(mul(world, float4(normal, 0)));
    //float4 wpos = mul(world, pos);
    
    ////�s�N�Z���V�F�[�_�[�ɓn���l
    //VSOutput output;
    
    //output.svpos = mul(mul(viewproj, world), pos);
    //output.worldpos = wpos;
    //output.normal = wnormal.xyz;
    //output.uv = uv;
    
    PS_IN result = (PS_IN) 0;
    //result.pos = mul(g_xCamera_VP, float4(pos, 1));
    result.uv = uv;
    return result;
}