#include"Object3D.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL,float2 uv : TEXCOORD)
{ 
    //法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    
    //ピクセルシェーダーに渡す値
    VSOutput output; 
    
    output.svpos = mul(mul(viewproj, world), pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    return output;
    
}