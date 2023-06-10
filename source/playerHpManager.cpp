/*******************************************************************************
*
*	�^�C�g���F	�v���C���[HP����X�e�[�g�N���X
*	�t�@�C���F	playerHpManager.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerHpManager.h"
#include "playerManager.h"
#include "manager.h"
#include "scene.h"
#include "gameObject.h"
#include "plate2dRenderer.h"

void PlayerHpManager::Update()
{
	if ((float)m_PlayerParameter->m_Hp != m_FillRate)
	{
		// ��ԏ���
		FillInterporation();
	}
}

void PlayerHpManager::CreateUi(PlayerParameter* Parameter)
{
	m_PlayerParameter = Parameter;
	m_FillRate = (float)m_PlayerParameter->m_Hp;

	string objName;
	GameObject* frame;
	GameObject* inside;
	Plate2DRenderer* plate2d;

	for (int i = 0; i < m_PlayerParameter->m_HpMax; i++)
	{
		objName = m_PLAYER_HP_MANAGER_OBJ_NAME + "_" + to_string(i + 1);

		frame = Manager::GetScene()->AddGameObject(objName + "_Frame");
		frame->m_LocalPosition = { m_UI_DISTANCE * i, 0.0f, 0.0f };
		frame->SetParent(m_AttachObject);
		frame->AddComponent<Plate2DRenderer>()->Init("PlayerHp_Frame", m_UI_SIZE);

		inside = Manager::GetScene()->AddGameObject(objName + "_Inside");
		inside->SetParent(frame);
		plate2d = inside->AddComponent<Plate2DRenderer>();
		plate2d->Init("PlayerHp_Inside", m_UI_SIZE);

		if (i > m_PlayerParameter->m_Hp - 1)
		{
			plate2d->SetTexCoordDistanceX(0.0f);
		}
	}
}

void PlayerHpManager::FillInterporation()
{
	float difference = (float)m_PlayerParameter->m_Hp - m_FillRate;
	int targetUiIndex = (int)m_FillRate;
	
	if (targetUiIndex == m_PlayerParameter->m_HpMax)
	{
		targetUiIndex = m_PlayerParameter->m_HpMax - 1;
	}

	// ��ԗʂ��v�Z
	float amount;
	if (fabs(difference) > m_FILL_INTERPOLATION_THRESHOLD)
	{
		amount = difference * m_FILL_INTERPOLATION_RATE;
	}
	else
	{
		amount = difference;
	}

	//printf("%f              \n", amount);

	if (amount < 0.0f)
	{// HP����
		FillDecrease(targetUiIndex, amount);
	}
	else
	{// HP�㏸
		FillRise(targetUiIndex, amount);
	}

	m_FillRate += amount;
}

void PlayerHpManager::FillDecrease(int UiIndex, float Amount)
{
	Plate2DRenderer* plate2d = m_AttachObject->
		GetChild(UiIndex)->GetChild(0)->GetComponent<Plate2DRenderer>();

	if (UiIndex >= m_FillRate + Amount)
	{
		float remainAmount = Amount + plate2d->GetTexCoordDistance().x;

		SetFill(plate2d, 0.0f);
		UiStop(UiIndex);	// ���S�ɏ������̂ŐU�����~

		if (UiIndex > 0)
		{
			plate2d = m_AttachObject->
				GetChild(UiIndex - 1)->GetChild(0)->GetComponent<Plate2DRenderer>();
			SetFill(plate2d, 1.0f + remainAmount);
		}
	}
	else
	{
		SetFill(plate2d, plate2d->GetTexCoordDistance().x + Amount);
		UiShake(UiIndex);
	}
}

void PlayerHpManager::FillRise(int UiIndex, float Amount)
{
	Plate2DRenderer* plate2d = m_AttachObject->
		GetChild(UiIndex)->GetChild(0)->GetComponent<Plate2DRenderer>();

	if (UiIndex + 1 <= m_FillRate + Amount)
	{
		float remainAmount = Amount - (1.0f - plate2d->GetTexCoordDistance().x);

		SetFill(plate2d, 1.0f);

		if (UiIndex < m_PlayerParameter->m_HpMax - 1)
		{
			plate2d = m_AttachObject->
				GetChild(UiIndex + 1)->GetChild(0)->GetComponent<Plate2DRenderer>();
			SetFill(plate2d, remainAmount);
		}
	}
	else
	{
		SetFill(plate2d, plate2d->GetTexCoordDistance().x + Amount);
	}
}

void PlayerHpManager::SetFill(Plate2DRenderer* Plate, float Rate)
{
	Plate->SetTexCoordDistanceX(Rate);
	Plate->SetSizeX(Rate * m_UI_SIZE.x);
}

void PlayerHpManager::UiShake(int UiIndex)
{
	D3DXVECTOR2 pos = Math::Random({ m_SHAKE_LENGTH * 2, m_SHAKE_LENGTH * 2 });
	pos -= { m_SHAKE_LENGTH, m_SHAKE_LENGTH };

	m_AttachObject->GetChild(UiIndex)->m_LocalPosition = {
		m_UI_DISTANCE * UiIndex + pos.x, pos.y, 0.0f
	};
}

void PlayerHpManager::UiStop(int UiIndex)
{
	m_AttachObject->GetChild(UiIndex)->m_LocalPosition = {
		m_UI_DISTANCE * UiIndex, 0.0f, 0.0f
	};
}
