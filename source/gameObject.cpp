/*******************************************************************************
*
*	タイトル：	ゲームオブジェクトクラス
*	ファイル：	gameObject.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "gameObject.h"
#include "component.h"
#include "manager.h"

#ifdef _DEBUG
#include "debugManager.h"
#include "debugWindow_Hierarchy.h"
#endif

void GameObject::Init(string name, int tag)
{
	// 初期化
	m_Name = name;
	m_Tag = tag;
	m_LocalPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LocalRotation = Math::GetQuaternionIdentity();
	m_LocalScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void GameObject::Uninit()
{
	// コンポーネントを全て解放
	for (Component* c : m_ComponentList)
	{
		c->Destroy();
	}

	// 自身に関する親子関係を全て削除
	SetParent(NULL);

	list<GameObject*> tempChild = m_Child;
	for (GameObject* obj : tempChild)
	{
		obj->SetParent(NULL);
	}
}

/*******************************************************************************
*	コンポーネントの削除確認
*******************************************************************************/
void GameObject::CheckComponentDestroy()
{
	// 削除予約のあったコンポーネントを削除する
	m_ComponentList.remove_if([&](Component* component)
	{
		if (component->GetIsDestroy())
		{
#ifdef _DEBUG
			if (component->GetDrawGroup() == Component::DrawGroup::Draw2D)
				{ m_AttachDraw2dComponentCount--; }
#endif
			component->Destroy();
			return true;
		}
		else
		{
			return false;
		}
	});
}

/*******************************************************************************
*	自身とその子オブジェクトの削除を予約
*******************************************************************************/
void GameObject::SetDestroy()
{
	// 自身の削除を予約
	m_IsDestroy = true;

	// 子オブジェクトの削除を予約
	for (GameObject* obj : m_Child)
	{
		obj->SetDestroy();
	}
}

/*******************************************************************************
*	削除処理
*******************************************************************************/
void GameObject::Destroy()
{
#ifdef _DEBUG
	UninitDebug();
#endif
	Uninit();
	delete this;
}

/*******************************************************************************
*	マトリクスを取得
*******************************************************************************/
D3DXMATRIX GameObject::GetMatrix()
{
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_LocalScale.x, m_LocalScale.y, m_LocalScale.z);
	D3DXMatrixRotationQuaternion(&rot, &m_LocalRotation);
	D3DXMatrixTranslation(&trans, m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z);
	return scale * rot * trans;
}

/*******************************************************************************
*	正面方向の単位ベクトルを取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetForward()
{
	D3DXMATRIX rot;
	D3DXQUATERNION q = GetWorldRotation();
	D3DXMatrixRotationQuaternion(&rot, &q);

	D3DXVECTOR3 forward = { rot._31, rot._32, rot._33 };

	return forward;
}

/*******************************************************************************
*	上方向の単位ベクトルを取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetUp()
{
	D3DXMATRIX rot;
	D3DXQUATERNION q = GetWorldRotation();
	D3DXMatrixRotationQuaternion(&rot, &q);

	D3DXVECTOR3 up = { rot._21, rot._22, rot._23 };

	return up;
}

/*******************************************************************************
*	右方向の単位ベクトルを取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetRight()
{
	D3DXMATRIX rot;
	D3DXQUATERNION q = GetWorldRotation();
	D3DXMatrixRotationQuaternion(&rot, &q);

	D3DXVECTOR3 right = { rot._11, rot._12, rot._13 };

	return right;
}

/*******************************************************************************
*	自身のローカルマトリクスの原点におけるワールドマトリクス
*	(親のワールドマトリクス)取得
*******************************************************************************/
D3DXMATRIX GameObject::GetLocalMatrixOrigin()
{
	// 親が存在する場合再帰的にマトリックスを求める
	if (m_Parent)
	{
		return m_Parent->GetWorldMatrix();
	}
	else
	{
		return Math::GetMatrixIdentity();
	}
}

/*******************************************************************************
*	ワールドマトリクス取得
*******************************************************************************/
D3DXMATRIX GameObject::GetWorldMatrix(D3DXMATRIX matrix)
{
	// 親が存在する場合再帰的にマトリックスを求める
	if (m_Parent)
	{
		return m_Parent->GetWorldMatrix(matrix * GetMatrix());
	}
	else
	{
		return matrix * GetMatrix();
	}
}

/*******************************************************************************
*	自身のローカル座標の原点におけるワールド座標
*	(親のワールド座標)取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetLocalPositionOrigin()
{
	// 親が存在する場合再帰的に座標を求める
	if (m_Parent)
	{
		return m_Parent->GetWorldPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

/*******************************************************************************
*	ワールド座標取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetWorldPosition(D3DXVECTOR3 position)
{
	// 親が存在する場合再帰的に座標を求める
	if (m_Parent)
	{
		return m_Parent->GetWorldPosition(position + m_LocalPosition);
	}
	else
	{
		return position + m_LocalPosition;
	}
}

/*******************************************************************************
*	ワールド座標設定
*******************************************************************************/
void GameObject::SetWorldPosition(D3DXVECTOR3 position)
{
	// 親のワールド座標から引いて求める
	if (m_Parent)
	{
		m_LocalPosition = position - m_Parent->GetWorldPosition();
	}
	else
	{
		m_LocalPosition = position;
	}
}

/*******************************************************************************
*	自身のローカルローテーションの原点におけるワールドローテーション
*	(親のワールドローテーション)取得
*******************************************************************************/
D3DXQUATERNION GameObject::GetLocalRotationOrigin()
{
	// 親が存在する場合再帰的に回転を求める
	if (m_Parent)
	{
		return m_Parent->GetWorldRotation(Math::GetQuaternionIdentity());
	}
	else
	{
		return Math::GetQuaternionIdentity();
	}
}

/*******************************************************************************
*	ワールドローテーション取得
*******************************************************************************/
D3DXQUATERNION GameObject::GetWorldRotation(D3DXQUATERNION rotation)
{
	// 親が存在する場合再帰的に回転を求める
	if (m_Parent)
	{
		return m_Parent->GetWorldRotation(rotation * m_LocalRotation);
	}
	else
	{
		return rotation * m_LocalRotation;
	}
}

/*******************************************************************************
*	ワールドローテーション設定
*******************************************************************************/
void GameObject::SetWorldRotation(D3DXQUATERNION rotation)
{
	D3DXVECTOR3 euler = Math::QuaternionToEulerAngle(rotation);
	D3DXVECTOR3 eulerParent = Math::QuaternionToEulerAngle(m_Parent->GetWorldRotation());

	// 親のワールドスケールから引いて求める
	if (m_Parent)
	{
		m_LocalRotation = Math::EulerAngleToQuaternion(euler - eulerParent);
	}
	else
	{
		m_LocalRotation = rotation;
	}
}

/*******************************************************************************
*	自身のローカルスケールの原点におけるワールドスケール
*	(親のワールドスケール)取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetLocalScaleOrigin()
{
	// 親が存在する場合再帰的に拡縮を求める
	if (m_Parent)
	{
		return m_Parent->GetWorldScale(D3DXVECTOR3());
	}
	else
	{
		return D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
}

/*******************************************************************************
*	ワールドスケール取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetWorldScale(D3DXVECTOR3 scale)
{
	D3DXVECTOR3 s = { scale.x * m_LocalScale.x, scale.y * m_LocalScale.y, scale.z * m_LocalScale.z };

	// 親が存在する場合再帰的に拡縮を求める
	if (m_Parent)
	{
		return m_Parent->GetWorldScale(s);
	}
	else
	{
		return s;
	}
}

/*******************************************************************************
*	ワールドスケール設定
*******************************************************************************/
void GameObject::SetWorldScale(D3DXVECTOR3 scale)
{
	// 親のワールドスケールから引いて求める
	if (m_Parent)
	{
		m_LocalScale = scale - m_Parent->GetWorldScale();
	}
	else
	{
		m_LocalScale = scale;
	}
}

/*******************************************************************************
*	全てのコンポーネントを取得する
*******************************************************************************/
list<Component*> GameObject::GetComponentAll()
{
	return m_ComponentList;
}

/*******************************************************************************
*	一番親のオブジェクトを取得
*******************************************************************************/
GameObject* GameObject::GetRoot()
{
	if (m_Parent)
	{
		return m_Parent->GetRoot();
	}
	else
	{
		return this;
	}
}

/*******************************************************************************
*	親オブジェクト設定
*******************************************************************************/
void GameObject::SetParent(GameObject* parent, int removeOption, int setOption)
{
	// 前の親オブジェクトから自身の情報を削除
	if (m_Parent)
	{
		CheckRemoveOption(removeOption, m_Parent);
		m_Parent->RemoveChild(this);
	}

	// 新しい親オブジェクトに自身の情報を登録
	if (parent)
	{
		CheckSetOption(setOption, parent);
		parent->AddChild(this);
	}

	// 親を変更
	m_Parent = parent;
}

/*******************************************************************************
*	子オブジェクトを添え字を指定して取得
*******************************************************************************/
GameObject* GameObject::GetChild(int index)
{
	if (m_Child.size() < (unsigned int)index + 1)
	{
		return NULL;
	}

	auto itr = m_Child.begin();
	for (int i = 0; i < index; i++)
	{
		itr++;
	}

	return (*itr);
}

/*******************************************************************************
*	自身がアクティブであるかを取得
*******************************************************************************/
bool GameObject::GetIsActive()
{
	if (m_Parent)
	{
		return m_Parent->GetIsActive();
	}
	else
	{
		return m_IsActive;
	}
}

/*******************************************************************************
*	オブジェクトの静的設定を変更する
*******************************************************************************/
void GameObject::SetIsStatic(bool value)
{
	if (m_IsStatic == value) { return; }

	if (value)
	{
		Manager::GetScene()->SetStatic(this);
	}
	else
	{
		Manager::GetScene()->SetDynamic(this);
	}
	
	m_IsStatic = value;
}

/*******************************************************************************
*	オブジェクトの名前を変更
*******************************************************************************/
void GameObject::SetName(string newName)
{
	Manager::GetScene()->SetObjectName(this, newName);
	m_Name = newName;
}

/*******************************************************************************
*	正面方向の単位ベクトルを取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetForwardInverse()
{
	D3DXMATRIX rot;
	D3DXQUATERNION q = GetWorldRotation();
	D3DXQuaternionInverse(&q, &q);
	D3DXMatrixRotationQuaternion(&rot, &q);

	D3DXVECTOR3 forward = { rot._31, rot._32, rot._33 };

	return forward;
}

/*******************************************************************************
*	上方向の単位ベクトルを取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetUpInverse()
{
	D3DXMATRIX rot;
	D3DXQUATERNION q = GetWorldRotation();
	D3DXQuaternionInverse(&q, &q);
	D3DXMatrixRotationQuaternion(&rot, &q);

	D3DXVECTOR3 up = { rot._21, rot._22, rot._23 };

	return up;
}

/*******************************************************************************
*	右方向の単位ベクトルを取得
*******************************************************************************/
D3DXVECTOR3 GameObject::GetRightInverse()
{
	D3DXMATRIX rot;
	D3DXQUATERNION q = GetWorldRotation();
	D3DXQuaternionInverse(&q, &q);
	D3DXMatrixRotationQuaternion(&rot, &q);

	D3DXVECTOR3 right = { rot._11, rot._12, rot._13 };

	return right;
}

void GameObject::CheckRemoveOption(int option, GameObject* parent)
{
	if (option & (int)SetParentOption::KeepPos)
	{
		m_LocalPosition += parent->GetWorldPosition();
	}
	if (option & (int)SetParentOption::KeepScale)
	{
		D3DXVECTOR3 s = parent->GetWorldScale();
		m_LocalScale = {
			m_LocalScale.x * s.x,
			m_LocalScale.y * s.y,
			m_LocalScale.z * s.z,
		};
	}
}

void GameObject::CheckSetOption(int option, GameObject* parent)
{
	if (option & (int)SetParentOption::KeepPos)
	{
		D3DXMATRIX m = GetWorldMatrix();
		D3DXVECTOR3 thisPos = { m._41, m._42, m._43 };
		m = parent->GetWorldMatrix();
		D3DXVECTOR3 parentPos = { m._41, m._42, m._43 };

		D3DXVECTOR3 pos = thisPos - parentPos;
		m_LocalPosition = 
			pos.x * parent->GetRightInverse() +
			pos.y * parent->GetUpInverse() +
			pos.z * parent->GetForwardInverse();
	}

	if (option & (int)SetParentOption::KeepScale)
	{
		D3DXVECTOR3 s = parent->GetWorldScale();
		m_LocalScale = {
			m_LocalScale.x / s.x,
			m_LocalScale.y / s.y,
			m_LocalScale.z / s.z,
		};
	}
}

/*******************************************************************************
*	デバッグ用関数
*******************************************************************************/
#ifdef _DEBUG
void GameObject::DrawInspector()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Info"))
	{
		string temp;

		ImGui::Checkbox("IsActive", &m_IsActive);

		ImGui::SameLine();
		ImGui::Checkbox("IsStatic", &m_IsStatic);

		ImGui::SameLine();
		temp = m_IsPreDestroy ? " " : "Destroy";
		ImGui::Checkbox(temp.c_str(), &m_IsPreDestroy);

		if (m_IsPreDestroy)
		{
			ImGui::SameLine();
			if (ImGui::Button("Destroy"))
			{
				SetDestroy();
			}
		}

		{
			D3DXVECTOR3 r = Math::QuaternionToEulerAngle(m_LocalRotation);
			r = Math::RadianToDegree(r);

			float pos[3] = { m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z };
			float rot[3] = { r.x, r.y, r.z };
			float scl[3] = { m_LocalScale.x, m_LocalScale.y, m_LocalScale.z };

			ImGui::DragFloat3("LocalPosition", pos, 0.1f, -10000.0f, 10000.0f, "%.3f");
			ImGui::DragFloat3("LocalRotation", rot, 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiSliderFlags_NoInput);
			ImGui::DragFloat3("LocalScale", scl, 0.1f, -10000.0f, 10000.0f, "%.3f");

			m_LocalPosition = { pos[0], pos[1], pos[2] };
			//m_LocalRotation = Math::EulerAngleToQuaternion(Math::DegreeToRadian({ rot[0], rot[1], rot[2] }));
			m_LocalScale = { scl[0], scl[1], scl[2] };
		}
	
		{
			D3DXVECTOR3 p = GetWorldPosition();
			D3DXVECTOR3 r = Math::QuaternionToEulerAngle(GetWorldRotation());
			r = Math::RadianToDegree(r);
			D3DXVECTOR3 s = GetWorldScale();

			float pos[3] = { p.x, p.y, p.z };
			float rot[3] = { r.x, r.y, r.z };
			float scl[3] = { s.x, s.y, s.z };

			ImGui::DragFloat3("WorldPosition", pos, 0.1f, -10000.0f, 10000.0f, "%.3f");
			ImGui::DragFloat3("WorldRotation", rot, 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiSliderFlags_NoInput);
			ImGui::DragFloat3("WorldScale", scl, 0.1f, -10000.0f, 10000.0f, "%.3f");

			m_LocalPosition += D3DXVECTOR3(pos[0], pos[1], pos[2]) - p;
			//m_LocalRotation *= Math::EulerAngleToQuaternion(Math::DegreeToRadian(D3DXVECTOR3(rot[0], rot[1], rot[2]) - r));
			m_LocalScale += D3DXVECTOR3(scl[0], scl[1], scl[2]) - s;
		}
	}
}

void GameObject::InitDebug()
{
	//-------------------
	// バッファ設定
	//-------------------
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VERTEX_3D) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VERTEX_3D* vertex = DBG_NEW VERTEX_3D[2];
	if (m_Parent)
	{
		CreateVertex(vertex);
	}

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	// バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	delete[] vertex;

	// シェーダー設定
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "debugLineVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "debugLinePS.cso");
}

void GameObject::UninitDebug()
{
	DebugWindow_Hierarchy::DeleteObj(this);

	m_VertexBuffer->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_VertexLayout->Release();
}

void GameObject::DrawDebugParentLine2d()
{
	if (m_AttachDraw2dComponentCount > 0)
	{
		DrawDebugParentLine();
	}
}

void GameObject::DrawDebugParentLine3d()
{
	if (m_AttachDraw2dComponentCount == 0)
	{
		DrawDebugParentLine();
	}
}

void GameObject::DrawDebugParentLine()
{
	if (!DebugManager::m_IsParentLine || !m_Parent)
	{
		return;
	}

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	D3DXMATRIX world;
	world = Math::GetMatrixIdentity();
	Renderer::SetWorldMatrix(&world);

	// 頂点情報書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	CreateVertex((VERTEX_3D*)msr.pData);
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	Renderer::SetMaterial(material);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(2, 0);
}

void GameObject::CreateVertex(VERTEX_3D* vertex)
{
	D3DXMATRIX thisMat = GetWorldMatrix();
	D3DXMATRIX parentMat = m_Parent->GetWorldMatrix();

	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR d = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	D3DXVECTOR2 c = D3DXVECTOR2(0.0f, 0.0f);

	vertex[0].Position = D3DXVECTOR3(thisMat._41, thisMat._42, thisMat._43);
	vertex[0].Normal = n;
	vertex[0].Diffuse = d;
	vertex[0].TexCoord = c;

	vertex[1].Position = D3DXVECTOR3(parentMat._41, parentMat._42, parentMat._43);
	vertex[1].Normal = n;
	vertex[1].Diffuse = d;
	vertex[1].TexCoord = c;
}
#endif
