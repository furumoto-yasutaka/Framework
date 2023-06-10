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

RWStructuredBuffer<PARTICLE_PARAM> ParticleParam : register(u0);    // �e�p�[�e�B�N���̃p�����[�^
AppendStructuredBuffer<uint> DeadList : register(u1);               // �p�[�e�B�N���̃v�[��(�ǉ�)
ConsumeStructuredBuffer<uint> ParticlePool : register(u2);          // �p�[�e�B�N���̃v�[��(�폜)
RWStructuredBuffer<uint> ParticleCount : register(u3);              // �v�[���ɂ���c��̃p�[�e�B�N����
RWStructuredBuffer<uint> RandomSeed : register(u4);                 // �����z��̃C���f�b�N�X

StructuredBuffer<PARTICLE_DEFAULT_PARAM> ParticleDefaultParam : register(t0);   // �p�[�e�B�N���̏����l
StructuredBuffer<float> DeltaTime : register(t1);                   // �t���[���Ԏ���
StructuredBuffer<float> RandomTable : register(t2);                 // �������������z��

// �����擾
float RandomFloat()
{
    float result = RandomTable[RandomSeed[0] % 1000];
    RandomSeed[0] = RandomSeed.IncrementCounter();

    return result;
}

// �����擾(Float2)
float2 RandomFloat2()
{
    return float2(RandomFloat(), RandomFloat());
}

// �����擾(Float3)
float3 RandomFloat3()
{
    return float3(RandomFloat(), RandomFloat(), RandomFloat());
}

// �p�[�e�B�N���̏�����
[numthreads(1, 1, 1)]
void Init(uint3 id : SV_GroupID)
{
    ParticleParam[id.x].IsActive = false;
    
    DeadList.Append(id.x);
    ParticleCount[0] = ParticleCount.IncrementCounter();
}

// �p�[�e�B�N���̐���(1�x�̌Ăяo����1��)
[numthreads(1, 1, 1)]
void Emit()
{
    // �p�[�e�B�N���̎c�ʂ�0�ɂȂ����琶�����s��Ȃ�
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

// �p�[�e�B�N���̍X�V(1�x�̌Ăяo����1��)
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
