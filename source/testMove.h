/*******************************************************************************
*
*	�^�C�g���F	�f���p�I�u�W�F�N�g�ړ��X�N���v�g
*	�t�@�C���F	testMove.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestMove : public NotDrawComponent
{
private:
	D3DXVECTOR3 m_FirstPos;
	D3DXVECTOR3 m_MoveRange;
	float m_Time = 0.0f;

public:
	TestMove(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestMove() {}

	void Init(D3DXVECTOR3 moveRange);
	void Uninit() override {}
	void Update() override;
};
