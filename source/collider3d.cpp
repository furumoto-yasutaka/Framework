/*******************************************************************************
*
*	タイトル：	3Dコリジョン基底クラス
*	ファイル：	collider3d.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "collider3d.h"
#include "collision3dManager.h"
#include "treeCell.h"
#include "gameObject.h"

#include <string>
#include <optional>

#ifdef _DEBUG
#include "debugWindow_Hierarchy.h"
#endif

void Collider3D::Init(D3DXVECTOR3 offset, bool isTrigger, bool isStatic)
{
	m_Offset = offset;
	m_IsTrigger = isTrigger;
	m_IsStatic = isStatic;

	m_ThisCellData = Collision3DManager::RegistCollision(this);
}

void Collider3D::Uninit()
{
	delete m_OnContactEnter;
	delete m_OnContactStay;
	delete m_OnContactExit;

	m_ThisCellData->Remove();
	Collision3DManager::RemoveTreeCellData(m_ThisCellData);
}

void Collider3D::Collision(Collider3D* collider)
{
	switch (collider->GetId())
	{
		using enum ColliderId;
	case AABB:
		CollisionAABB(reinterpret_cast<AABBCollider3D*>(collider));
		break;
	case OBB:
		CollisionOBB(reinterpret_cast<OBBCollider3D*>(collider));
		break;
	case Sphere:
		CollisionSphere(reinterpret_cast<SphereCollider3D*>(collider));
		break;
	case Capsule:
		CollisionCapsule(reinterpret_cast<CapsuleCollider3D*>(collider));
		break;
	}

#ifdef _DEBUG
	Collision3DManager::AddCollisionCount();
#endif
}

D3DXVECTOR3 Collider3D::GetColliderWorldPos()
{
	D3DXMATRIX result, world, offset;
	world = m_AttachObject->GetWorldMatrix();
	D3DXMatrixTranslation(&offset, m_Offset.x, m_Offset.y, m_Offset.z);
	result = offset * world;

	return { result._41, result._42, result._43 };
}

void Collider3D::CallOnContactEnter(Collider3D* collider)
{
	if (m_OnContactEnter)
	{
		m_OnContactEnter->Execute(collider);
	}
}
void Collider3D::CallOnContactStay(Collider3D* collider)
{
	if (m_OnContactStay)
	{
		m_OnContactStay->Execute(collider);
	}
}
void Collider3D::CallOnContactExit(Collider3D* collider)
{
	if (m_OnContactExit)
	{
		m_OnContactExit->Execute(collider);
	}
}

void Collider3D::AddContactList(Collider3D* collider)
{
	// 自分がトリガーモードではなく、相手はトリガーモードの場合は何もしない
	if (!m_IsTrigger && collider->GetIsTrigger())
	{
		return;
	}
	else
	{
		m_ContactList.push_back(collider);
	}
}

void Collider3D::RemoveRemainElement(Collider3D* collider)
{
	m_RemainList.remove(collider);
}

void Collider3D::CheckCallBack()
{
	auto cItr = m_ContactList.begin();

	while (cItr != m_ContactList.end())
	{
		if ((*cItr)->GetIsTrigger() && !m_IsTrigger)
		{
			cItr = m_ContactList.erase(cItr);
		}
		else
		{
			auto findItr = find(m_RemainList.begin(), m_RemainList.end(), (*cItr));
			if (findItr != m_RemainList.end())
			{
				CallOnContactStay((*cItr));
				m_RemainList.erase(findItr);
			}
			else
			{
				CallOnContactEnter((*cItr));
			}

			cItr++;
		}
	}

	for (Collider3D* c : m_RemainList)
	{
		CallOnContactExit(c);
	}

	m_RemainList = m_ContactList;
	m_ContactList.clear();
}

float Collider3D::CalcDirectionVectorSeparateAxisLength(
	D3DXVECTOR3 a_DirVec, D3DXVECTOR3 a_DirVecNormal,
	D3DXVECTOR3 b_DirVecRight, D3DXVECTOR3 b_DirVecUp, D3DXVECTOR3 b_DirVecForward,
	D3DXVECTOR3 distance)
{
	float r1 = D3DXVec3Length(&a_DirVec);
	float r2 = LenSegOnSeparateAxis(&a_DirVecNormal,
		&b_DirVecRight, &b_DirVecUp, &b_DirVecForward);
	float length = fabs(D3DXVec3Dot(&distance, &a_DirVecNormal));
	float bite = length - (r1 + r2);

	return bite;
}

float Collider3D::CalcCrossSeparateAxisLength(D3DXVECTOR3 cross,
	D3DXVECTOR3 a_DirVec1, D3DXVECTOR3 a_DirVec2,
	D3DXVECTOR3 b_DirVec1, D3DXVECTOR3 b_DirVec2,
	D3DXVECTOR3 distance)
{
	float r1 = LenSegOnSeparateAxis(&cross, &a_DirVec1, &a_DirVec2);
	float r2 = LenSegOnSeparateAxis(&cross, &b_DirVec1, &b_DirVec2);
	float length = fabs(D3DXVec3Dot(&distance, &cross));
	float bite = length - (r1 + r2);

	return bite;
}

float Collider3D::LenSegOnSeparateAxis(D3DXVECTOR3* sep,
	D3DXVECTOR3* edge1, D3DXVECTOR3* edge2, D3DXVECTOR3* edge3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは正規化されていること
	float r1 = fabsf(D3DXVec3Dot(sep, edge1));
	float r2 = fabsf(D3DXVec3Dot(sep, edge2));
	float r3 = edge3 ? (fabsf(D3DXVec3Dot(sep, edge3))) : 0;
	return r1 + r2 + r3;
}

void Collider3D::SetMinBiteAndMinBiteAxis(
	float bite, D3DXVECTOR3 biteAxis, float& minBite, D3DXVECTOR3& minBiteAxis)
{
	if (minBite > bite)
	{
		minBite = bite;
		minBiteAxis = biteAxis;
	}
}

void Collider3D::SetVelocityRef(D3DXVECTOR3* velocityRef, optional<float> damp)
{
	m_VelocityRef = velocityRef;

	if (m_Damp)
	{
		m_Damp = *damp;
	}
}

void Collider3D::ReflectionVelocityRef(D3DXVECTOR3 base)
{
	if (m_VelocityRef)
	{
		D3DXVECTOR3 vel = { 0.0f, 0.0f, 0.0f };

		if (base.x != 0.0f)
		{
			if (m_VelocityRef->x / base.x < 0.0f)
			{
				vel.x = -m_VelocityRef->x + -m_VelocityRef->x * m_Damp;
			}
		}
		if (base.y != 0.0f)
		{
			if (m_VelocityRef->y / base.y < 0.0f)
			{
				vel.y = -m_VelocityRef->y + -m_VelocityRef->y * m_Damp;
			}
		}
		if (base.z != 0.0f)
		{
			if (m_VelocityRef->z / base.z < 0.0f)
			{
				vel.z = -m_VelocityRef->z + -m_VelocityRef->z * m_Damp;
			}
		}

		*m_VelocityRef += vel;
	}
}

void Collider3D::DeleteVelocityRef()
{
	m_VelocityRef = NULL;
}

void Collider3D::SetIsTrigger(bool value)
{
	m_IsTrigger = value;

	// 衝突相手がトリガーで、自身トリガーではない場合は、
	// コールバックは発生しないのでリストから削除する
	if (!m_IsTrigger)
	{
		m_RemainList.remove_if([](Collider3D* c) { return c->GetIsTrigger(); });
	}
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void Collider3D::DrawInspector()
{
	float tempVec3[3];

	tempVec3[0] = m_Offset.x;
	tempVec3[1] = m_Offset.y;
	tempVec3[2] = m_Offset.z;
	if (ImGui::InputFloat3("Offset", tempVec3))
	{
		m_Offset = { tempVec3[0], tempVec3[1], tempVec3[2] };
	}

	ImGui::Text("IsTrigger : %s", (m_IsTrigger ? "true" : "false"));

	ImGui::Text("IsStatic : %s", (m_IsStatic ? "true" : "false"));

	if (ImGui::TreeNode("ContactList"))
	{
		// 衝突数表示
		ImGui::Text("HitCount : %d", m_RemainList.size());

		{
			// 衝突したオブジェクトを表示する窓を作成
			float height = m_CHILD_WINDOW_BLANK_HEIGHT + m_CHILD_WINDOW_LINE_HEIGHT * (m_RemainList.size() + 1);
			if (height > m_CHILD_WINDOW_HEIGHT_MAX)
			{
				height = m_CHILD_WINDOW_HEIGHT_MAX;
			}
			ImGui::BeginChildFrame(ImGui::GetID((void*)0), ImVec2(m_CHILD_WINDOW_WIDTH, height), ImGuiWindowFlags_NoTitleBar);

			// 構成を説明するテキスト
			ImGui::Text("HierarchyIndex : ObjectName");

			ImGui::Separator();

			// 衝突したオブジェクトのヒエラルキー上のインデックスとオブジェクト名を表示
			for (auto itr = m_RemainList.begin(); itr != m_RemainList.end(); itr++)
			{
				optional<int> index = DebugWindow_Hierarchy::GetObjectIndex((*itr)->GetAttachObject()).value();
				if (index)
				{
					ImGui::Text("%d : %s", index.value(), (*itr)->GetAttachObject()->GetName().c_str());
				}
			}

			ImGui::EndChildFrame();
		}

		ImGui::TreePop();
	}
}
#endif
