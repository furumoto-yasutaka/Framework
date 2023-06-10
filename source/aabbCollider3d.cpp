/*******************************************************************************
*
*	タイトル：	軸並行境界3Dコリジョンコンポーネント
*	ファイル：	aabbCollider3d.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "aabbCollider3d.h"
#include "gameObject.h"
#include "obbCollider3D.h"
#include "sphereCollider3d.h"
#include "capsuleCollider3d.h"

#include <algorithm>

#ifdef _DEBUG
#include "debugManager.h"
#endif

void AABBCollider3D::Init(D3DXVECTOR3 offset, D3DXVECTOR3 radius,
	bool isTrigger, bool isStatic)
{
	Collider3D::Init(offset, isTrigger, isStatic);

	m_LocalRadius = radius;
}

void AABBCollider3D::Uninit()
{
	Collider3D::Uninit();
}

void AABBCollider3D::CollisionAABB(AABBCollider3D* collider)
{
	D3DXVECTOR3 r = GetWorldRadius() + collider->GetWorldRadius();
	D3DXVECTOR3 distance = GetColliderWorldPos() - collider->GetColliderWorldPos();
	D3DXVECTOR3 check = { fabs(distance.x) - r.x, fabs(distance.y) - r.y, fabs(distance.z) - r.z };

	if (check.x < 0 && check.y < 0 && check.z < 0)
	{
		// 衝突が確定
		AddContactList(collider);
		collider->AddContactList(this);

		// お互いがトリガーではない場合押し出し処理を行う
		if (!m_IsTrigger && !collider->GetIsTrigger())
		{
			// 押し出し方向がプラスかマイナスか
			D3DXVECTOR3 f = {
				distance.x != 0.0f ? distance.x / fabs(distance.x) : 0.0f,
				distance.y != 0.0f ? distance.y / fabs(distance.y) : 0.0f,
				distance.z != 0.0f ? distance.z / fabs(distance.z) : 0.0f,
			};

			// 押し出しに使用する値
			D3DXVECTOR3 adjust = {
				check.x * f.x,
				check.y * f.y,
				check.z * f.z,
			};

			// 一番食い込みが少ない軸を押し出し・反射方向に設定
			float max = check.x;
			if (check.y > max) { max = check.y; }
			if (check.z > max) { max = check.z; }
			if (max > check.x) { adjust.x = 0.0f; f.x = 0.0f; }
			if (max > check.y) { adjust.y = 0.0f; f.y = 0.0f; }
			if (max > check.z) { adjust.z = 0.0f; f.z = 0.0f; }

			// 反射処理
			ReflectionVelocityRef(f);
			collider->ReflectionVelocityRef(-f);

			// 押し出しを反映
			if (!m_IsStatic && !collider->GetIsStatic())
			{
				m_AttachObject->GetRoot()->m_LocalPosition -= adjust * 0.5f;
				collider->GetAttachObject()->GetRoot()->m_LocalPosition += adjust * 0.5f;
			}
			else if (!m_IsStatic)
			{
				m_AttachObject->GetRoot()->m_LocalPosition -= adjust;
			}
			else
			{
				collider->GetAttachObject()->GetRoot()->m_LocalPosition += adjust;
			}
		}
	}
}

void AABBCollider3D::CollisionOBB(OBBCollider3D* collider)
{
	D3DXVECTOR3 thisRadius = GetWorldRadius();
	D3DXVECTOR3 targetRadius = collider->GetWorldRadius();
	D3DXVECTOR3 thisCenter = GetColliderWorldPos();
	D3DXVECTOR3 targetCenter = collider->GetColliderWorldPos();
	D3DXVECTOR3 distance = targetCenter - thisCenter;

	// 各方向ベクトルの確保
	// 末尾が1：右ベクトル
	// 末尾が2：上ベクトル
	// 末尾が3：前ベクトル
	D3DXVECTOR3 nThis1 = m_AttachObject->GetRight(),
		This1 = nThis1 * thisRadius.x,
		nThis2 = m_AttachObject->GetUp(),
		This2 = nThis2 * thisRadius.y,
		nThis3 = m_AttachObject->GetForward(),
		This3 = nThis3 * thisRadius.z;
	D3DXVECTOR3 nTarget1 = collider->GetAttachObject()->GetRight(),
		Target1 = nTarget1 * targetRadius.x,
		nTarget2 = collider->GetAttachObject()->GetUp(),
		Target2 = nTarget2 * targetRadius.y,
		nTarget3 = collider->GetAttachObject()->GetForward(),
		Target3 = nTarget3 * targetRadius.z;

	float minBite;
	D3DXVECTOR3 minBiteAxis;

	// 分離軸：This1
	float bite = CalcDirectionVectorSeparateAxisLength(This1, nThis1, Target1, Target2, Target3, distance);
	if (bite > 0.0f) { return; }
	minBite = bite;
	minBiteAxis = nThis1;

	// 分離軸：This2
	bite = CalcDirectionVectorSeparateAxisLength(This2, nThis2, Target1, Target2, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, nThis2, minBite, minBiteAxis);

	// 分離軸：This3
	bite = CalcDirectionVectorSeparateAxisLength(This3, nThis3, Target1, Target2, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, nThis3, minBite, minBiteAxis);

	// 分離軸：Target1
	bite = CalcDirectionVectorSeparateAxisLength(Target1, nTarget1, This1, This2, This3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, nTarget1, minBite, minBiteAxis);

	// 分離軸：Target2
	bite = CalcDirectionVectorSeparateAxisLength(Target2, nTarget2, This1, This2, This3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, nTarget2, minBite, minBiteAxis);

	// 分離軸：Target3
	bite = CalcDirectionVectorSeparateAxisLength(Target3, nTarget3, This1, This2, This3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, nTarget3, minBite, minBiteAxis);

	// 分離軸：This1とTarget1の外積
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &nThis1, &nTarget1);
	bite = CalcCrossSeparateAxisLength(cross, This2, This3, Target2, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This1とTarget2の外積
	D3DXVec3Cross(&cross, &nThis1, &nTarget2);
	bite = CalcCrossSeparateAxisLength(cross, This2, This3, Target1, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This1とTarget3の外積
	D3DXVec3Cross(&cross, &nThis1, &nTarget3);
	bite = CalcCrossSeparateAxisLength(cross, This2, This3, Target1, Target2, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This2とTarget1の外積
	D3DXVec3Cross(&cross, &nThis2, &nTarget1);
	bite = CalcCrossSeparateAxisLength(cross, This1, This3, Target2, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This2とTarget2の外積
	D3DXVec3Cross(&cross, &nThis2, &nTarget2);
	bite = CalcCrossSeparateAxisLength(cross, This1, This3, Target1, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This2とTarget3の外積
	D3DXVec3Cross(&cross, &nThis2, &nTarget3);
	bite = CalcCrossSeparateAxisLength(cross, This1, This3, Target1, Target2, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This3とTarget1の外積
	D3DXVec3Cross(&cross, &nThis3, &nTarget1);
	bite = CalcCrossSeparateAxisLength(cross, This1, This2, Target2, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This3とTarget2の外積
	D3DXVec3Cross(&cross, &nThis3, &nTarget2);
	bite = CalcCrossSeparateAxisLength(cross, This1, This2, Target1, Target3, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 分離軸：This3とTarget3の外積
	D3DXVec3Cross(&cross, &nThis3, &nTarget3);
	bite = CalcCrossSeparateAxisLength(cross, This1, This2, Target1, Target2, distance);
	if (bite > 0.0f) { return; }
	SetMinBiteAndMinBiteAxis(bite, cross, minBite, minBiteAxis);

	// 衝突が確定
	AddContactList(collider);
	collider->AddContactList(this);

	// お互いがトリガーではない場合押し出し処理を行う
	if (!m_IsTrigger && !collider->GetIsTrigger())
	{
		// 反射処理
		ReflectionVelocityRef(minBiteAxis);
		collider->ReflectionVelocityRef(-minBiteAxis);

		// 押し出しを反映
		D3DXVECTOR3 adjust = minBiteAxis * minBite;
		if (!m_IsStatic && !collider->GetIsStatic())
		{
			m_AttachObject->GetRoot()->m_LocalPosition -= adjust * 0.5f;
			collider->GetAttachObject()->GetRoot()->m_LocalPosition += adjust * 0.5f;
		}
		else if (!m_IsStatic)
		{
			m_AttachObject->GetRoot()->m_LocalPosition -= adjust;
		}
		else
		{
			collider->GetAttachObject()->GetRoot()->m_LocalPosition += adjust;
		}
	}
}

void AABBCollider3D::CollisionSphere(SphereCollider3D* collider)
{
	float len = 0;
	D3DXVECTOR3 boxCenter = GetColliderWorldPos();
	D3DXVECTOR3 boxMinPoint = boxCenter - GetWorldRadius();
	D3DXVECTOR3 boxMaxPoint = boxCenter + GetWorldRadius();
	D3DXVECTOR3 sphereCenter = collider->GetColliderWorldPos();
	D3DXVECTOR3 distance = sphereCenter - boxCenter;
	D3DXVECTOR3 check = { 0.0f, 0.0f, 0.0f };

	// X軸
	if (sphereCenter.x < boxMinPoint.x)
	{
		check.x = sphereCenter.x - boxMinPoint.x;
	}
	else if (sphereCenter.x > boxMaxPoint.x)
	{
		check.x = sphereCenter.x - boxMaxPoint.x;
	}

	// Y軸
	if (sphereCenter.y < boxMinPoint.y)
	{
		check.y = sphereCenter.y - boxMinPoint.y;
	}
	else if (sphereCenter.y > boxMaxPoint.y)
	{
		check.y = sphereCenter.y - boxMaxPoint.y;
	}

	// Z軸
	if (sphereCenter.z < boxMinPoint.z)
	{
		check.z = sphereCenter.z - boxMinPoint.z;
	}
	else if (sphereCenter.z > boxMaxPoint.z)
	{
		check.z = sphereCenter.z - boxMaxPoint.z;
	}

	len += D3DXVec3LengthSq(&check);

	float bite = len - collider->GetWorldRadius() * collider->GetWorldRadius();
	if (bite < 0.0f)
	{
		// 衝突が確定
		AddContactList(collider);
		collider->AddContactList(this);

		// お互いがトリガーではない場合押し出し処理を行う
		if (!m_IsTrigger && !collider->GetIsTrigger())
		{
			// 押し出し方向がプラスかマイナスか
			D3DXVECTOR3 f = {
				distance.x != 0.0f ? distance.x / fabs(distance.x) : 0.0f,
				distance.y != 0.0f ? distance.y / fabs(distance.y) : 0.0f,
				distance.z != 0.0f ? distance.z / fabs(distance.z) : 0.0f,
			};
			// 押し出しに使用する値
			D3DXVECTOR3 adjust = check;

			// 一番食い込みが少ない軸を押し出し・反射方向に設定
			D3DXVECTOR3 absCheck = { fabs(check.x), fabs(check.y), fabs(check.z) };
			float min = absCheck.x;
			if (absCheck.y < min) { min = absCheck.y; }
			if (absCheck.z < min) { min = absCheck.z; }
			if (min < absCheck.x) { adjust.x = 0.0f; f.x = 0.0f; }
			if (min < absCheck.y) { adjust.y = 0.0f; f.y = 0.0f; }
			if (min < absCheck.z) { adjust.z = 0.0f; f.z = 0.0f; }

			// 反射処理
			if (m_VelocityRef)
			{
				D3DXVECTOR3 vel = {
					m_VelocityRef->x * (f.x + f.x * m_Damp),
					m_VelocityRef->y * (f.y + f.y * m_Damp),
					m_VelocityRef->z * (f.z + f.z * m_Damp),
				};
				*m_VelocityRef += vel;
			}

			// 押し出し処理
			if (!m_IsStatic && !collider->GetIsStatic())
			{
				m_AttachObject->GetRoot()->m_LocalPosition -= adjust * 0.5f;
				collider->GetAttachObject()->GetRoot()->m_LocalPosition += adjust * 0.5f;
			}
			else if (!m_IsStatic)
			{
				m_AttachObject->GetRoot()->m_LocalPosition -= adjust;
			}
			else
			{
				collider->GetAttachObject()->GetRoot()->m_LocalPosition += adjust;
			}
		}
	}
}

void AABBCollider3D::CollisionCapsule(CapsuleCollider3D* collider)
{
	 
}

D3DXVECTOR3 AABBCollider3D::GetMortonMinPos()
{
	return GetColliderWorldPos() - GetWorldRadius();
}

D3DXVECTOR3 AABBCollider3D::GetMortonMaxPos()
{
	return GetColliderWorldPos() + GetWorldRadius();
}

D3DXVECTOR3 AABBCollider3D::GetWorldRadius()
{
	D3DXVECTOR3 scale = m_AttachObject->GetWorldScale();
	D3DXVECTOR3 radius = {
		m_LocalRadius.x * scale.x,
		m_LocalRadius.y * scale.y,
		m_LocalRadius.z * scale.z,
	};

	return radius;
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void AABBCollider3D::DrawInspector()
{
	if (ImGui::CollapsingHeader("AABBCollider3D"))
	{
		float tempVec3[3];

		tempVec3[0] = m_LocalRadius.x;
		tempVec3[1] = m_LocalRadius.y;
		tempVec3[2] = m_LocalRadius.z;
		if (ImGui::InputFloat3("LocalRadius", tempVec3))
		{
			m_LocalRadius = { tempVec3[0], tempVec3[1], tempVec3[2] };
		}

		D3DXVECTOR3 v = GetWorldRadius();
		tempVec3[0] = v.x;
		tempVec3[1] = v.y;
		tempVec3[2] = v.z;
		ImGui::InputFloat3("WorldRadius", tempVec3, "%.3f", ImGuiInputTextFlags_ReadOnly);

		Collider3D::DrawInspector();
	}
}

void AABBCollider3D::InitDebug()
{
	//-------------------
	// バッファ設定
	//-------------------
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3DXVECTOR3 boxMinPoint = m_Offset - m_LocalRadius;
	D3DXVECTOR3 boxMaxPoint = m_Offset + m_LocalRadius;

	D3DXVECTOR3 vertexPos[2][4];
	// 上段
	vertexPos[0][0] = D3DXVECTOR3(boxMinPoint.x, boxMaxPoint.y, boxMinPoint.z);
	vertexPos[0][1] = D3DXVECTOR3(boxMaxPoint.x, boxMaxPoint.y, boxMinPoint.z);
	vertexPos[0][2] = D3DXVECTOR3(boxMaxPoint.x, boxMaxPoint.y, boxMaxPoint.z);
	vertexPos[0][3] = D3DXVECTOR3(boxMinPoint.x, boxMaxPoint.y, boxMaxPoint.z);
	// 下段
	vertexPos[1][0] = D3DXVECTOR3(boxMinPoint.x, boxMinPoint.y, boxMinPoint.z);
	vertexPos[1][1] = D3DXVECTOR3(boxMaxPoint.x, boxMinPoint.y, boxMinPoint.z);
	vertexPos[1][2] = D3DXVECTOR3(boxMaxPoint.x, boxMinPoint.y, boxMaxPoint.z);
	vertexPos[1][3] = D3DXVECTOR3(boxMinPoint.x, boxMinPoint.y, boxMaxPoint.z);

	VERTEX_3D vertex[4];
	D3D11_SUBRESOURCE_DATA sd;
	for (int i = 0; i < 4; i++)
	{
		CreateVertex(vertex, i, vertexPos);

		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		// バッファ生成
		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[i]);
	}

	// シェーダー設定
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "debugLineVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "debugLinePS.cso");
}

void AABBCollider3D::UninitDebug()
{
	for (int i = 0; i < 4; i++)
	{
		m_VertexBuffer[i]->Release();
	}
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_VertexLayout->Release();
}

void AABBCollider3D::DrawDebug3d()
{
	if (!DebugManager::m_IsCollisionFrame) return;

	D3DXVECTOR3 boxMinPoint = m_Offset - m_LocalRadius;
	D3DXVECTOR3 boxMaxPoint = m_Offset + m_LocalRadius;

	D3DXVECTOR3 vertexPos[2][4];
	// 上段
	vertexPos[0][0] = D3DXVECTOR3(boxMinPoint.x, boxMaxPoint.y, boxMinPoint.z);
	vertexPos[0][1] = D3DXVECTOR3(boxMaxPoint.x, boxMaxPoint.y, boxMinPoint.z);
	vertexPos[0][2] = D3DXVECTOR3(boxMaxPoint.x, boxMaxPoint.y, boxMaxPoint.z);
	vertexPos[0][3] = D3DXVECTOR3(boxMinPoint.x, boxMaxPoint.y, boxMaxPoint.z);
	// 下段
	vertexPos[1][0] = D3DXVECTOR3(boxMinPoint.x, boxMinPoint.y, boxMinPoint.z);
	vertexPos[1][1] = D3DXVECTOR3(boxMaxPoint.x, boxMinPoint.y, boxMinPoint.z);
	vertexPos[1][2] = D3DXVECTOR3(boxMaxPoint.x, boxMinPoint.y, boxMaxPoint.z);
	vertexPos[1][3] = D3DXVECTOR3(boxMinPoint.x, boxMinPoint.y, boxMaxPoint.z);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	// ※AABBなので回転を反映しない
	D3DXMATRIX world, pm, sm;
	D3DXVECTOR3 p = m_AttachObject->GetWorldPosition();
	D3DXVECTOR3 s = m_AttachObject->GetWorldScale();
	D3DXMatrixTranslation(&pm, p.x, p.y, p.z);
	D3DXMatrixScaling(&sm, s.x, s.y, s.z);
	world = sm * pm;
	Renderer::SetWorldMatrix(&world);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	Renderer::SetMaterial(material);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	for (int i = 0; i < 4; i++)
	{
		// 頂点情報書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer[i], 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);
		CreateVertex((VERTEX_3D*)msr.pData, i, vertexPos);
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[i], 0);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[i], &stride, &offset);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}

void AABBCollider3D::CreateVertex(VERTEX_3D* vertex, int index, D3DXVECTOR3 vertexPos[2][4])
{
	int next = (index + 1) % 4;

	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR d = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	D3DXVECTOR2 c = D3DXVECTOR2(0.0f, 0.0f);

	vertex[0].Position = vertexPos[0][index];
	vertex[0].Normal = n;
	vertex[0].Diffuse = d;
	vertex[0].TexCoord = c;

	vertex[1].Position = vertexPos[0][next];
	vertex[1].Normal = n;
	vertex[1].Diffuse = d;
	vertex[1].TexCoord = c;

	vertex[2].Position = vertexPos[1][next];
	vertex[2].Normal = n;
	vertex[2].Diffuse = d;
	vertex[2].TexCoord = c;

	vertex[3].Position = vertexPos[1][index];
	vertex[3].Normal = n;
	vertex[3].Diffuse = d;
	vertex[3].TexCoord = c;
}
#endif
