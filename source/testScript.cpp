/*******************************************************************************
*
*	�^�C�g���F	�e�X�g�p�R���|�[�l���g
*	�t�@�C���F	testScript.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "testScript.h"
#include "gameObject.h"

void TestScript::Init()
{

}

void TestScript::Update()
{
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.5f);
}
