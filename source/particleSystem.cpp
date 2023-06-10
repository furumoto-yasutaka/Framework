/*******************************************************************************
*
*	タイトル：	パーティクル生成コンポーネント
*	ファイル：	particleSystem.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "particleSystem.h"
#include "gameObject.h"
#include "camera.h"
#include "particleEmitChecker.h"
#include "textureContainer.h"

#include <tchar.h>

ParticleSystem::EmitParam::EmitParam()
{
	IsLooping = true;
	Duration = 1.0f;
	DurationCount = Duration;
	MaxParticles = 10;
	ParticleCount = 0;
	Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SimulationSpace = SimulationSpaceId::World;
	StopAction = EmitterStopActionId::ObjectDestroy;
	StopCallback = NULL;
}

ParticleSystem::ParticleDefaultParam::ParticleDefaultParam()
{
	Lifetime = 0.0f;
	StartSize = Math::MinMax<float>();
	StartSize.Min = 1.0f;
	StartSize.Max = 1.0f;
	Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	StartVelocity = Math::MinMax<D3DXVECTOR3>();
	StartVelocity.Min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	StartVelocity.Max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Gravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	TexName = "";
	Tex = NULL;
	Color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

ParticleSystem::ParticleDefaultParam::ParticleDefaultParam(
	float lifetime, Math::MinMax<float> startSize,
	Math::MinMax<D3DXVECTOR3> startVelocity,
	D3DXVECTOR3 gravity, string texName, D3DXVECTOR4 color)
{
	this->Lifetime = lifetime;
	this->StartSize = startSize;
	this->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->StartVelocity = startVelocity;
	this->Gravity = gravity;
	this->TexName = texName;
	this->Tex = TextureContainer::GetTexture_InName(TexName);
	this->Color = color;
}

void ParticleSystem::Init(float generateDelay, bool isPlayOnAwake,
	ParticleSystem::EmitParam emitterParam,
	ParticleDefaultParam particleDefaltParam,
	ParticleEmitChackerBase* particleEmitChacker)
{
	if (isPlayOnAwake)
	{
		if (generateDelay <= 0.0f)
		{// 再生遅延がない
			// 再生状態で初期化する
			Play();
		}
		else
		{// 再生遅延がある
			SetInvoke(&ParticleSystem::Play, generateDelay);
		}
	}

	m_EmitterParam = emitterParam;
	m_ParticleDefaultParam = particleDefaltParam;
	m_ParticleDefaultParam.Position += m_EmitterParam.Offset;
	m_EmitChacker = particleEmitChacker;
	m_EmitChacker->SetParticleSystem(this);
	
	CreateShader();
	CreateVertex();
	CreateBuffer();

	// ↓コンピュートシェーダーでまとめて初期化↓
	RunComputeShader_Init();
}

void ParticleSystem::Uninit()
{
	if (m_EmitterParam.StopCallback) { delete m_EmitterParam.StopCallback; }

	delete m_EmitChacker;

	// コンピュートシェーダー
	m_ComputeShader_Init->Release();
	m_ComputeShader_Emit->Release();
	m_ComputeShader_Update->Release();

	// コンピュートシェーダーリソース
	m_ParticleParamBuf->Release();
	m_ParticleParamSRV->Release();
	m_ParticleParamUAV->Release();

	m_ParticlePoolBuf->Release();
	m_ParticlePoolUAV->Release();

	m_ParticlePoolCountBuf->Release();
	m_ParticlePoolCountUAV->Release();

	m_ParticleDefaultParamBuf->Release();
	m_ParticleDefaultParamSRV->Release();

	m_DeltaTimeBuf->Release();
	m_DeltaTimeSRV->Release();

	m_RandomTableBuf->Release();
	m_RandomTableSRV->Release();

	m_RandomSeedBuf->Release();
	m_RandomSeedUAV->Release();

	// 描画用リソース
	m_VertexBuffer->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_VertexLayout->Release();
}

void ParticleSystem::Update()
{
	if (m_IsPlay)
	{
		// フレーム間時間の更新
		D3D11_MAPPED_SUBRESOURCE subRes;
		Renderer::GetDeviceContext()->Map(m_DeltaTimeBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
		float* bufType = (float*)subRes.pData;
		bufType[0] = Time::GetDeltaTimeSec();
		Renderer::GetDeviceContext()->Unmap(m_DeltaTimeBuf, 0);

		if (m_EmitterParam.DurationCount > 0.0f)
		{
			float elopsedTime = m_EmitterParam.Duration - m_EmitterParam.DurationCount;
			m_EmitChacker->CheckGenerate(elopsedTime);

			m_EmitterParam.DurationCount -= Time::GetDeltaTimeSec();
		}
		else
		{
			EndSystem();
		}

		// ↓コンピュートシェーダーでパーティクルをまとめて更新↓
		RunComputeShader_Update();
	}
}

void ParticleSystem::LateUpdate()
{
	CheckInvoke();
}

void ParticleSystem::Draw3d()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ビルボード
	Camera* cam = Manager::GetScene()->GetActiveCamera();
	D3DXMATRIX view = cam->GetViewMatrix();
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	D3DXMATRIX world, scale, trans;
	D3DXVECTOR3 scl = m_AttachObject->GetWorldScale();
	D3DXVECTOR3 pos = m_AttachObject->GetWorldPosition();
	D3DXMatrixScaling(&scale, scl.x, scl.y, scl.z);
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);

	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// アルファトゥカパレッジ設定
	Renderer::SetAlphaToCaverage(true);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// テクスチャ設定
	ID3D11ShaderResourceView* resource = m_ParticleDefaultParam.Tex->GetResource();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource); // テクスチャ設定（あれば）

	// 頂点設定
	Renderer::GetDeviceContext()->VSSetShaderResources(0, 1, &m_ParticleParamSRV);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawInstanced(4, m_EmitterParam.MaxParticles, 0, 0);


	// アルファトゥカパレッジ設定
	Renderer::SetAlphaToCaverage(false);
}

void ParticleSystem::Play()
{
	m_IsPlay = true;
}

void ParticleSystem::Pause()
{
	m_IsPlay = false;
}

void ParticleSystem::Stop()
{
	EndSystem();
}

void ParticleSystem::CreateShader()
{
	// 頂点シェーダー
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "gpuParticleVS.cso");

	// ピクセルシェーダー
	Renderer::CreatePixelShader(&m_PixelShader, "gpuParticlePS.cso");

	// コンピュートシェーダー
	ID3D10Blob* blob = NULL;

	UINT Flag = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	Flag |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif
	const char* filename = "hlsl\\gpuParticleCS.hlsl";
	const char* ver = "cs_5_0";

	// コンピュートシェーダーの初期化処理
	HRESULT hr = D3DX11CompileFromFile(
		_T(filename), NULL, NULL, "Init", ver, Flag, 0, NULL, &blob, NULL, NULL);
	if (FAILED(hr)) { return; }
	Renderer::GetDevice()->CreateComputeShader(
		blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_ComputeShader_Init);

	// コンピュートシェーダーの生成処理
	hr = D3DX11CompileFromFile(
		_T(filename), NULL, NULL, "Emit", ver, Flag, 0, NULL, &blob, NULL, NULL);
	if (FAILED(hr)) { return; }
	Renderer::GetDevice()->CreateComputeShader(
		blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_ComputeShader_Emit);

	// コンピュートシェーダーの更新処理
	hr = D3DX11CompileFromFile(
		_T(filename), NULL, NULL, "Update", ver, Flag, 0, NULL, &blob, NULL, NULL);
	if (FAILED(hr)) { return; }
	Renderer::GetDevice()->CreateComputeShader(
		blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_ComputeShader_Update);

	blob->Release();
}

void ParticleSystem::CreateVertex()
{
	// パーティクルの頂点情報
	VERTEX_3D vertex[4];
	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// パーティクルの頂点情報バッファ
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//bd.Usage = D3D11_USAGE_DYNAMIC;
		//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}
}

ID3D11ShaderResourceView* ParticleSystem::CreateSRV(ID3D11Buffer* buf, int numElements)
{
	// シェーダーリソースビューに変換
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.BufferEx.NumElements = numElements;

	ID3D11ShaderResourceView* srv = NULL;
	Renderer::GetDevice()->CreateShaderResourceView(buf, &SRVDesc, &srv);
	assert(srv);

	return srv;
}

ID3D11UnorderedAccessView* ParticleSystem::CreateUAV(ID3D11Buffer* buf, int numElements, unsigned int bufferFlag)
{
	// アンオーダーアクセスビューに変換
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.Buffer.NumElements = numElements;
	UAVDesc.Buffer.Flags = bufferFlag;

	ID3D11UnorderedAccessView* uav = NULL;
	Renderer::GetDevice()->CreateUnorderedAccessView(buf, &UAVDesc, &uav);
	assert(uav);

	return uav;
}

void ParticleSystem::CreateBuffer()
{	
	// それぞれのバッファを確保

	// m_ParticleParamBuf
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bd.ByteWidth = sizeof(ParticleParam) * m_EmitterParam.MaxParticles;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(ParticleParam);
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_ParticleParamBuf);
		assert(m_ParticleParamBuf);
	}
	// m_ParticlePoolBuf
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.ByteWidth = sizeof(unsigned int) * m_EmitterParam.MaxParticles;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(unsigned int);
		bd.Usage = D3D11_USAGE_DEFAULT;

		Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_ParticlePoolBuf);
		assert(m_ParticlePoolBuf);
	}
	// m_ParticlePoolCountBuf
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.ByteWidth = sizeof(unsigned int);
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(unsigned int);
		bd.Usage = D3D11_USAGE_DEFAULT;

		unsigned int p = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &p;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticlePoolCountBuf);
		assert(m_ParticlePoolCountBuf);
	}
	// m_ParticleDefaultParamBuf
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.ByteWidth = sizeof(ParticleDefaultParamSRV);
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(ParticleDefaultParamSRV);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ParticleDefaultParamSRV p;
		p.Lifetime = m_ParticleDefaultParam.Lifetime;
		p.StartSizeMax = m_ParticleDefaultParam.StartSize.Max;
		p.StartSizeMin = m_ParticleDefaultParam.StartSize.Min;
		p.Position = m_ParticleDefaultParam.Position;
		p.StartVelocityMax = m_ParticleDefaultParam.StartVelocity.Max;
		p.StartVelocityMin = m_ParticleDefaultParam.StartVelocity.Min;
		p.Gravity = m_ParticleDefaultParam.Gravity;
		p.Color = m_ParticleDefaultParam.Color;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &p;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticleDefaultParamBuf);
		assert(m_ParticleDefaultParamBuf);
	}
	// m_DeltaTimeBuf
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.ByteWidth = sizeof(unsigned int);
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(unsigned int);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_DeltaTimeBuf);
		assert(m_DeltaTimeBuf);
	}
	// m_RandomTableBuf
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.ByteWidth = sizeof(float) * 1000;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(float);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		float p[1000];

		for (int i = 0; i < 1000; i++)
		{
			p[i] = Math::Random(1.0f);
		}

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &p;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_RandomTableBuf);
		assert(m_RandomTableBuf);
	}
	// m_RandomSeedBuf
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.ByteWidth = sizeof(unsigned int);
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = sizeof(unsigned int);
		bd.Usage = D3D11_USAGE_DEFAULT;

		unsigned int p = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &p;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_RandomSeedBuf);
		assert(m_RandomSeedBuf);
	}

	// シェーダーリソースビューに変換
	m_ParticleParamSRV = CreateSRV(m_ParticleParamBuf, m_EmitterParam.MaxParticles);
	m_ParticleDefaultParamSRV = CreateSRV(m_ParticleDefaultParamBuf, 1);
	m_DeltaTimeSRV = CreateSRV(m_DeltaTimeBuf, 1);
	m_RandomTableSRV = CreateSRV(m_RandomTableBuf, 1000);

	// アンオーダーアクセスビューに変換
	m_ParticleParamUAV = CreateUAV(m_ParticleParamBuf, m_EmitterParam.MaxParticles, D3D11_BUFFER_UAV_FLAG_APPEND);
	m_ParticlePoolUAV = CreateUAV(m_ParticlePoolBuf, m_EmitterParam.MaxParticles, D3D11_BUFFER_UAV_FLAG_APPEND);
	m_ParticlePoolCountUAV = CreateUAV(m_ParticlePoolCountBuf, 1, D3D11_BUFFER_UAV_FLAG_COUNTER);
	m_RandomSeedUAV = CreateUAV(m_RandomSeedBuf, 1, D3D11_BUFFER_UAV_FLAG_COUNTER);
}

void ParticleSystem::RunComputeShader_Init()
{
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader_Init, NULL, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ParticleParamUAV, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, &m_ParticlePoolUAV, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(3, 1, &m_ParticlePoolCountUAV, NULL);

	Renderer::GetDeviceContext()->Dispatch(m_EmitterParam.MaxParticles, 1, 1);

	ResetComputeShader();
}

void ParticleSystem::RunConputeShader_Emit(int count)
{
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader_Emit, NULL, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ParticleParamUAV, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(2, 1, &m_ParticlePoolUAV, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(3, 1, &m_ParticlePoolCountUAV, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(4, 1, &m_RandomSeedUAV, NULL);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_ParticleDefaultParamSRV);
	Renderer::GetDeviceContext()->CSSetShaderResources(2, 1, &m_RandomTableSRV);

	Renderer::GetDeviceContext()->Dispatch(count, 1, 1);

	ResetComputeShader();
}

void ParticleSystem::RunComputeShader_Update()
{
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader_Update, NULL, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ParticleParamUAV, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, &m_ParticlePoolUAV, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(3, 1, &m_ParticlePoolCountUAV, NULL);
	Renderer::GetDeviceContext()->CSSetShaderResources(1, 1, &m_DeltaTimeSRV);

	Renderer::GetDeviceContext()->Dispatch(m_EmitterParam.MaxParticles, 1, 1);

	ResetComputeShader();
}

void ParticleSystem::ResetComputeShader()
{
	Renderer::GetDeviceContext()->CSSetShader(NULL, NULL, 0);
	ID3D11UnorderedAccessView* uav_null[1] = { NULL };
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, uav_null, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, uav_null, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(2, 1, uav_null, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(3, 1, uav_null, NULL);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(4, 1, uav_null, NULL);
	ID3D11ShaderResourceView* srv_null[1] = { NULL };
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, srv_null);
	Renderer::GetDeviceContext()->CSSetShaderResources(1, 1, srv_null);
	Renderer::GetDeviceContext()->CSSetShaderResources(2, 1, srv_null);
}

void ParticleSystem::CheckEmitStop()
{
	// エミッターの稼働時間を過ぎたのでループ設定ではない場合停止する
	if (m_EmitterParam.IsLooping)
	{
		m_EmitterParam.DurationCount = m_EmitterParam.Duration;
	}
	else
	{
		EndSystem();
	}
}

void ParticleSystem::EndSystem()
{
	switch (m_EmitterParam.StopAction)
	{
	case EmitterStopActionId::ComponentDisable:
		m_IsActive = false;
		break;
	case EmitterStopActionId::ObjectDestroy:
		m_AttachObject->SetDestroy();
		break;
	case EmitterStopActionId::ComponentDestroy:
		SetDestroy();
		break;
	}

	if (m_EmitterParam.StopCallback)
	{
		m_EmitterParam.StopCallback->Execute();
	}
}

// アンオーダードアクセスビューのバッファの内容を CPU から読み込み可能なバッファへコピーする
ID3D11Buffer* ParticleSystem::CreateAndCopyToDebugBuf(ID3D11Buffer* buffer)
{
	ID3D11Buffer* debugbuf = NULL;

	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	buffer->GetDesc(&BufferDesc);
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;  // CPU から読み込みできるように設定する
	BufferDesc.Usage = D3D11_USAGE_STAGING;             // GPU から CPU へのデータ転送 (コピー) をサポートするリソース
	BufferDesc.BindFlags = 0;
	BufferDesc.MiscFlags = 0;
	Renderer::GetDevice()->CreateBuffer(&BufferDesc, NULL, &debugbuf);

	Renderer::GetDeviceContext()->CopyResource(debugbuf, buffer);
	return debugbuf;
}
