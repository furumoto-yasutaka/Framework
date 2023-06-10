/*******************************************************************************
*
*	�^�C�g���F	���f���Ǘ��p�ÓI�N���X
*	�t�@�C���F	modelContainer.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "modelContainer.h"
#include "manager.h"
#include "scene.h"

#include <algorithm>

void ModelContainer::Init()
{
	// �f���p�A�Z�b�g
	{
		// �v���C���[
		AddModel("Coconut", "Game\\Coconut\\coconut.obj", 1.0f);
		AddModel("Player_Sphere", "Game\\Player\\PlayerSphere.obj", 1.0f);
		AddModel("Player_Cover", "Game\\Player\\PlayerCover.obj", 1.0f);
		AddModel("Player_WeaponConnection", "Game\\Player\\PlayerRodConnection.obj", 1.0f);
		AddModel("Player_Weapon", "Game\\Player\\PlayerWeapon.obj", 1.0f);

		// �G�l�~�[
		AddModel("Enemy_Egg", "Game\\Egg\\egg.obj", 0.5f);

		// �v���b�g�t�H�[��
		AddModel("Platform_Cube", "Game\\Platform\\Cube.obj", 1.0f);
	}

	// �X�J�C�h�[���p
	AddModel("SkyDome", "Common\\SkyDome\\SkyDome.obj", 1.0f);
}

void ModelContainer::Uninit()
{
	// ���ׂẴ��f�������
	auto itr = m_ModelInfo.begin();
	while (itr != m_ModelInfo.end())
	{
		delete itr->second;
		itr = m_ModelInfo.erase(itr);
	}
}

/*******************************************************************************
*	���f���擾(���̂Ō���)
*******************************************************************************/
Model* ModelContainer::GetModel_InName(string name)
{
	auto itr = m_ModelInfo.find(name);

	if (itr != m_ModelInfo.end())
	{
		return itr->second;
	}
	else
	{
		string str = string("���݂��Ȃ����̂̃��f���̎擾���v������܂����B\n") + name;
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("modelContainer:Error"),
			MB_OK | MB_ICONERROR);
		return NULL;
	}
}

/*******************************************************************************
*	���f���ǉ�
*******************************************************************************/
void ModelContainer::AddModel(string name, string path, float scale)
{
	string fullPath = "asset\\model\\" + path;

	// �v�f���}�b�v�ɑ��݂��邩�m�F
	if (m_ModelInfo.find(name) != m_ModelInfo.end())
	{
		string str = string("���łɑ��݂��閼�̂Ń��f���̒ǉ����v������܂����B\n") + fullPath + "\n���̃��f���͓ǂݍ��܂�܂���B";
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("modelContainer:Error"),
			MB_OK | MB_ICONERROR);
	}
	else
	{
		m_ModelInfo[name] = DBG_NEW Model(fullPath.c_str(), scale);
	}
}
