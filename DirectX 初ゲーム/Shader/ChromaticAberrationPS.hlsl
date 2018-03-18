// éQçlÅFhttp://hikita12312.hatenablog.com/entry/2017/10/06/000812

#define CHROMATIC_ABERRATION_ARRAY_NUM 5

static const float2 Resolution = float2(1280.0, 720.0);

static const float3 s_aChoromaticAberrationFilter[CHROMATIC_ABERRATION_ARRAY_NUM] =
{
    float3(0.0, 0.0, 0.5),
    float3(0.0, 0.25, 0.5),
    float3(0.0, 0.5, 0.0),
    float3(0.5, 0.25, 0.0),
    float3(0.5, 0.0, 0.0)
};

float time;
float strength;
float scale;

sampler2D sampler0;


void ChromaticAberrationPS(in float2 texCoord : TEXCOORD0, out float4 color : COLOR0)
{
    float2 offset = float2(1.0, 1.0) * 0.5;
    color = float4(0.0, 0.0, 0.0, 1.0);
    
    for (int i = 0; i < CHROMATIC_ABERRATION_ARRAY_NUM; ++i)
    {
        float effect = lerp(1.0, (1.0 - scale * i / CHROMATIC_ABERRATION_ARRAY_NUM), strength);
        float2 uv = ((texCoord - offset) * effect) + offset;
        uv.y += lerp(0.0, sin(texCoord.x + time * 10.0) * 0.001, strength);
        uv.x += lerp(0.0, sin(texCoord.y + time * 10.0) * 0.005, strength);

        float3 mask = s_aChoromaticAberrationFilter[i];
        float3 srcColor = tex2D(sampler0, uv).xyz;
        color.xyz += srcColor * mask;
    }
}