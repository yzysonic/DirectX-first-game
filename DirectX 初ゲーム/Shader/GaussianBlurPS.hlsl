// éQçlÅFhttps://wgld.org/d/webgl/w057.html

static const float2 Resolution = float2(1280.0, 720.0);

sampler2D sampler0;

bool is_horizon;
float weight[10];


void GaussianBlurPS(in float2 pos : VPOS, out float4 color : COLOR0)
{

    float2 tFrag = 1.0 / Resolution;
    float3 destColor = float3(0.0, 0.0, 0.0);

    if (is_horizon)
    {
        destColor += tex2D(sampler0, (pos + float2(-9.0, 0.0)) * tFrag).rgb * weight[9];
        destColor += tex2D(sampler0, (pos + float2(-8.0, 0.0)) * tFrag).rgb * weight[8];
        destColor += tex2D(sampler0, (pos + float2(-7.0, 0.0)) * tFrag).rgb * weight[7];
        destColor += tex2D(sampler0, (pos + float2(-6.0, 0.0)) * tFrag).rgb * weight[6];
        destColor += tex2D(sampler0, (pos + float2(-5.0, 0.0)) * tFrag).rgb * weight[5];
        destColor += tex2D(sampler0, (pos + float2(-4.0, 0.0)) * tFrag).rgb * weight[4];
        destColor += tex2D(sampler0, (pos + float2(-3.0, 0.0)) * tFrag).rgb * weight[3];
        destColor += tex2D(sampler0, (pos + float2(-2.0, 0.0)) * tFrag).rgb * weight[2];
        destColor += tex2D(sampler0, (pos + float2(-1.0, 0.0)) * tFrag).rgb * weight[1];
        destColor += tex2D(sampler0, (pos + float2(0.0, 0.0)) * tFrag).rgb * weight[0];
        destColor += tex2D(sampler0, (pos + float2(1.0, 0.0)) * tFrag).rgb * weight[1];
        destColor += tex2D(sampler0, (pos + float2(2.0, 0.0)) * tFrag).rgb * weight[2];
        destColor += tex2D(sampler0, (pos + float2(3.0, 0.0)) * tFrag).rgb * weight[3];
        destColor += tex2D(sampler0, (pos + float2(4.0, 0.0)) * tFrag).rgb * weight[4];
        destColor += tex2D(sampler0, (pos + float2(5.0, 0.0)) * tFrag).rgb * weight[5];
        destColor += tex2D(sampler0, (pos + float2(6.0, 0.0)) * tFrag).rgb * weight[6];
        destColor += tex2D(sampler0, (pos + float2(7.0, 0.0)) * tFrag).rgb * weight[7];
        destColor += tex2D(sampler0, (pos + float2(8.0, 0.0)) * tFrag).rgb * weight[8];
        destColor += tex2D(sampler0, (pos + float2(9.0, 0.0)) * tFrag).rgb * weight[9];
    }
    else
    {
        destColor += tex2D(sampler0, (pos + float2(0.0, -9.0)) * tFrag).rgb * weight[9];
        destColor += tex2D(sampler0, (pos + float2(0.0, -8.0)) * tFrag).rgb * weight[8];
        destColor += tex2D(sampler0, (pos + float2(0.0, -7.0)) * tFrag).rgb * weight[7];
        destColor += tex2D(sampler0, (pos + float2(0.0, -6.0)) * tFrag).rgb * weight[6];
        destColor += tex2D(sampler0, (pos + float2(0.0, -5.0)) * tFrag).rgb * weight[5];
        destColor += tex2D(sampler0, (pos + float2(0.0, -4.0)) * tFrag).rgb * weight[4];
        destColor += tex2D(sampler0, (pos + float2(0.0, -3.0)) * tFrag).rgb * weight[3];
        destColor += tex2D(sampler0, (pos + float2(0.0, -2.0)) * tFrag).rgb * weight[2];
        destColor += tex2D(sampler0, (pos + float2(0.0, -1.0)) * tFrag).rgb * weight[1];
        destColor += tex2D(sampler0, (pos + float2(0.0, 0.0)) * tFrag).rgb * weight[0];
        destColor += tex2D(sampler0, (pos + float2(0.0, 1.0)) * tFrag).rgb * weight[1];
        destColor += tex2D(sampler0, (pos + float2(0.0, 2.0)) * tFrag).rgb * weight[2];
        destColor += tex2D(sampler0, (pos + float2(0.0, 3.0)) * tFrag).rgb * weight[3];
        destColor += tex2D(sampler0, (pos + float2(0.0, 4.0)) * tFrag).rgb * weight[4];
        destColor += tex2D(sampler0, (pos + float2(0.0, 5.0)) * tFrag).rgb * weight[5];
        destColor += tex2D(sampler0, (pos + float2(0.0, 6.0)) * tFrag).rgb * weight[6];
        destColor += tex2D(sampler0, (pos + float2(0.0, 7.0)) * tFrag).rgb * weight[7];
        destColor += tex2D(sampler0, (pos + float2(0.0, 8.0)) * tFrag).rgb * weight[8];
        destColor += tex2D(sampler0, (pos + float2(0.0, 9.0)) * tFrag).rgb * weight[9];

    }

    color = float4(destColor, 1.0);

}