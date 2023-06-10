/*******************************************************************************
*
*	�^�C�g���F	Invoke���؃X�N���v�g
*	�t�@�C���F	testInvoke.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "testInvoke.h"
#include "gameObject.h"

void TestInvoke::Init()
{
	SetInvoke(&TestInvoke::Reset, m_Interval);
}

void TestInvoke::Update()
{
	m_AttachObject->m_LocalPosition.y -= m_Speed;
}

void TestInvoke::Reset()
{
	m_AttachObject->m_LocalPosition.y = 0.0f;
	SetInvoke(&TestInvoke::Reset, m_Interval);
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void TestInvoke::DrawInspector()
{
	if (ImGui::CollapsingHeader("TestInvoke"))
	{
		ImGui::SliderFloat("Interval", &m_Interval, 0.1f, 10.0f);

		ImGui::SliderFloat("Speed", &m_Speed, 0.001f, 0.1f);
	}
}
#endif
