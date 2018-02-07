float4x4 mtxView;
float4x4 mtxProjection;

void InstancingVS(
	in float4 m_1 : TEXCOORD1,
    in float4 m_2 : TEXCOORD2,
    in float4 m_3 : TEXCOORD3,
    in float4 m_4 : TEXCOORD4,

    inout float4 pos : POSITION,
    inout float4 color : COLOR
)
{
    float4x4 mtxWorld = float4x4(m_1, m_2, m_3, m_4);

    pos = mul(pos, mtxWorld);
    pos = mul(pos, mtxView);
    pos = mul(pos, mtxProjection);
}