/*******************************************************************************
*
*	�^�C�g���F	���f���Ǘ��p�ÓI�N���X
*	�t�@�C���F	modelContainer.h
*	�쐬�ҁF	�Ö{ �ח�
* 
*******************************************************************************/
#pragma once
#include "model.h"

#include <unordered_map>

class ModelContainer
{
private:
	// ���v�f
	// ����1	string�^	���f������(�L�[)
	// ����2	Model*�^	���f�����
	static inline unordered_map<string, Model*> m_ModelInfo;	// ���f�����X�g

private:
	ModelContainer() {}
public:
	static void Init();
	static void Uninit();

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//

	// ���f���擾(���̂Ō���)
	static Model* GetModel_InName(string name);
	/////////////////////////////
private:
	// ���f���ǉ�
	static void AddModel(string name, string path, float scale);
};
