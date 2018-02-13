sampler2D sampler0;

void MotionBlurPS(in float2 uv : TEXCOORD0, out float4 color : COLOR0)
{
    color = tex2D(sampler0, uv);
    color.a = max(color.a - 0.2f, 0.0f);
    color = float4(0, 0, 0, 0);
}