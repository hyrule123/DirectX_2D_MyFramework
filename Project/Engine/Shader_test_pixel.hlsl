#include "Shader_global_header.hlsli"
#include "Shader_test_header.hlsli"


#define COLOR_KEY g_vec4_0


// pixel shader
float4 test_PS(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
  
    if (g_int_0 == 0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vOutUV);
    else if (g_int_0 == 1)
        vColor = g_tex_0.Sample(g_sam_1, _in.vOutUV);
    
    if (all(vColor == g_vec4_0))
        discard;
    
    return vColor;
}