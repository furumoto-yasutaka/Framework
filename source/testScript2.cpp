/*******************************************************************************
*
*	�^�C�g���F	�e�X�g�p�R���|�[�l���g
*	�t�@�C���F	testScript2.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "testScript2.h"
#include "gameObject.h"

void TestScript2::Init()
{

}

void TestScript2::Update()
{
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.8f);
}
