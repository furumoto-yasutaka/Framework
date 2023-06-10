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

struct PARTICLE_DEFAULT_PARAM
{
    float Lifetime;
    float StartSizeMax;
    float StartSizeMin;
    float3 StartPosition;
    float3 StartVelocityMax;
    float3 StartVelocityMin;
    float3 Gravity;
    float4 Color;
};

RWStructuredBuffer<PARTICLE_PARAM> ParticleParam : register(u0);    // 各パーティクルのパラメータ
AppendStructuredBuffer<uint> DeadList : register(u1);               // パーティクルのプール(追加)
ConsumeStructuredBuffer<uint> ParticlePool : register(u2);          // パーティクルのプール(削除)
RWStructuredBuffer<uint> ParticleCount : register(u3);              // プールにある残りのパーティクル数
RWStructuredBuffer<uint> RandomSeed : register(u4);                 // 乱数配列のインデックス

StructuredBuffer<PARTICLE_DEFAULT_PARAM> ParticleDefaultParam : register(t0);   // パーティクルの初期値
StructuredBuffer<float> DeltaTime : register(t1);                   // フレーム間時間
StructuredBuffer<float> RandomTable : register(t2);                 // 乱数が入った配列

// 乱数取得
float RandomFloat()
{
    float result = RandomTable[RandomSeed[0] % 1000];
    RandomSeed[0] = RandomSeed.IncrementCounter();

    return result;
}

// 乱数取得(Float2)
float2 RandomFloat2()
{
    return float2(RandomFloat(), RandomFloat());
}

// 乱数取得(Float3)
float3 RandomFloat3()
{
    return float3(RandomFloat(), RandomFloat(), RandomFloat());
}

// パーティクルの初期化
[numthreads(1, 1, 1)]
void Init(uint3 id : SV_GroupID)
{
    ParticleParam[id.x].IsActive = false;
    
    DeadList.Append(id.x);
    ParticleCount[0] = ParticleCount.IncrementCounter();
}

// パーティクルの生成(1度の呼び出しで1つ)
[numthreads(1, 1, 1)]
void Emit()
{
    // パーティクルの残量が0になったら生成を行わない
    if (ParticleCount[0] > 0)
    {
        uint id = ParticlePool.Consume();
        ParticleCount[0] = ParticleCount.DecrementCounter();
    
        float2 seed = float2(id + 1, id + 2);
        float size = ParticleDefaultParam[0].StartSizeMin;
        size += (ParticleDefaultParam[0].StartSizeMax - ParticleDefaultParam[0].StartSizeMin) * RandomFloat();
        float3 vel = ParticleDefaultParam[0].StartVelocityMin;
        vel += (ParticleDefaultParam[0].StartVelocityMax - ParticleDefaultParam[0].StartVelocityMin) * RandomFloat3();
    
        PARTICLE_PARAM p;
        p.IsActive = true;
        p.Lifetime = ParticleDefaultParam[0].Lifetime;
        p.Size = size;
        p.Position = ParticleDefaultParam[0].StartPosition;
        p.Velocity = vel;
        p.Gravity = ParticleDefaultParam[0].Gravity;
        p.Color = ParticleDefaultParam[0].Color;
        ParticleParam[id] = p;
    }
}

// パーティクルの更新(1度の呼び出しで1つ)
[numthreads(1, 1, 1)]
void Update(uint3 id : SV_GroupID)
{
    PARTICLE_PARAM p = ParticleParam[id.x];
  
    if (p.IsActive)
    {
        p.Lifetime -= DeltaTime[0];
        p.Position += p.Velocity;
        p.Velocity += p.Gravity;
      
        if (p.Lifetime <= 0.0)
        {
            p.IsActive = false;
    
            DeadList.Append(id.x);
            ParticleCount[0] = ParticleCount.IncrementCounter();
        }
        ParticleParam[id.x] = p;
    }
}
