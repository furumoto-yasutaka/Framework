#include "common.hlsl"

struct PARTICLE_PARAM
{
    bool IsActive;
    float Lifetime;
    float Size;
    float3 Position;
    float3 Velocity;
    float3 Gravity;
    float4 Color;
};

StructuredBuffer<PARTICLE_PARAM> ParticleParam : register(t0);

float3 rotate(float3 p, float3 rotation)
{
    float3 a = normalize(rotation);
    float angle = length(rotation);
    if (abs(angle) < 0.001)
        return p;
    float s = sin(angle);
    float c = cos(angle);
    float r = 1.0 - c;
    float3x3 m = float3x3(
        a.x * a.x * r + c,
        a.y * a.x * r + a.z * s,
        a.z * a.x * r - a.y * s,
        a.x * a.y * r - a.z * s,
        a.y * a.y * r + c,
        a.z * a.y * r + a.x * s,
        a.x * a.z * r + a.y * s,
        a.y * a.z * r - a.x * s,
        a.z * a.z * r + c
    );
    return mul(m, p);
}

void main(in VS_IN In, out PS_IN Out)
{
    PARTICLE_PARAM p = ParticleParam[In.InstanceID];
    
    if (p.IsActive)
    {
        matrix wvp;
        wvp = mul(World, View);
        wvp = mul(wvp, Projection);
        
        float4 pos = In.Position;
        pos.xy *= p.Size;
        pos.xyz += p.Position;
        Out.Position = mul(pos, wvp);
        Out.WorldPosition = mul(In.Position, World);
        Out.Normal = In.Normal;
        Out.TexCoord = In.TexCoord;
        Out.Diffuse = In.Diffuse;
    }
    else
    {
        Out.Position = float4(0.0f, 0.0f, 0.0f, 0.0f);
        Out.WorldPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
        Out.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
        Out.TexCoord = float2(0.0f, 0.0f);
        Out.Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}