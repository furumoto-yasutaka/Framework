/*******************************************************************************
*
*	�^�C�g���F	�Z�[�u�f�[�^�Ǘ��p�ÓI�N���X
*	�t�@�C���F	savedataManager.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "savedata.h"

#include <fstream>

class SavedataManager
{
public:
	//static const int m_SAVE_DATA_NUM = 3;
	static inline SaveData m_SaveData;		// �Z�[�u�f�[�^���

private:
	SavedataManager() {}
public:
	static void Init();
	static void Uninit();

	// �Z�[�u
	static void Save();
	
private:
	// ���[�h
	static void Load(ifstream* ifs);

	// ����
	static SaveData Create();

	// ������
	static void Clear();
};
