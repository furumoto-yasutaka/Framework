/*******************************************************************************
*
*	�^�C�g���F	�Z�[�u�f�[�^�Ǘ��p�ÓI�N���X
*	�t�@�C���F	savedataManager.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "savedataManager.h"
#include "main.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

void SavedataManager::Init()
{
	// �t�@�C�����J��
	ifstream ifs("asset\\json\\savedata.json");

	// �ǂݍ��݂ɐ���������
	if (ifs){ Load(&ifs); }
	else	{ Clear(); }

	ifs.close();
}

void SavedataManager::Uninit()
{

}

/*******************************************************************************
*	���[�h
*******************************************************************************/
void SavedataManager::Load(ifstream* ifs)
{
	// �t�@�C����ǂݍ���
	json j;
	*ifs >> j;

	////------------------------
	//// �f�[�^���i�[
	////------------------------
	//m_SaveData.TargetBreaking.NormalTargetBreak = j["TargetBreaking"]["NormalTargetBreak"];
	//m_SaveData.TargetBreaking.RareTargetBreak = j["TargetBreaking"]["RareTargetBreak"];
	//m_SaveData.TargetBreaking.Score = j["TargetBreaking"]["Score"];

	//m_SaveData.KumiteNormal.BulletHit = j["KumiteNormal"]["BulletHit"];
	//m_SaveData.KumiteNormal.ChargeBulletHit = j["KumiteNormal"]["ChargeBulletHit"];
	//m_SaveData.KumiteNormal.DestroyEnemy = j["KumiteNormal"]["DestroyEnemy"];
	//m_SaveData.KumiteNormal.HitBalloon = j["KumiteNormal"]["HitBalloon"];
	//m_SaveData.KumiteNormal.Score = j["KumiteNormal"]["Score"];

	//m_SaveData.KumiteHard.BulletHit = j["KumiteHard"]["BulletHit"];
	//m_SaveData.KumiteHard.ChargeBulletHit = j["KumiteHard"]["ChargeBulletHit"];
	//m_SaveData.KumiteHard.DestroyEnemy = j["KumiteHard"]["DestroyEnemy"];
	//m_SaveData.KumiteHard.HitBalloon = j["KumiteHard"]["HitBalloon"];
	//m_SaveData.KumiteHard.Score = j["KumiteHard"]["Score"];
}

/*******************************************************************************
*	�Z�[�u
*******************************************************************************/
void SavedataManager::Save()
{
	//------------------------
	// �������ݗp�ϐ��Ɋi�[
	//------------------------
	json j;

	//j["TargetBreaking"] = {
	//	{"NormalTargetBreak", m_SaveData.TargetBreaking.NormalTargetBreak},
	//	{"RareTargetBreak", m_SaveData.TargetBreaking.RareTargetBreak},
	//	{"Score", m_SaveData.TargetBreaking.Score},
	//};
	//j["KumiteNormal"] = {
	//	{"BulletHit", m_SaveData.KumiteNormal.BulletHit },
	//	{"ChargeBulletHit", m_SaveData.KumiteNormal.ChargeBulletHit },
	//	{"DestroyEnemy", m_SaveData.KumiteNormal.DestroyEnemy },
	//	{"HitBalloon", m_SaveData.KumiteNormal.HitBalloon },
	//	{"Score", m_SaveData.KumiteNormal.Score },
	//};
	//j["KumiteHard"] = {
	//	{"BulletHit", m_SaveData.KumiteHard.BulletHit },
	//	{"ChargeBulletHit", m_SaveData.KumiteHard.ChargeBulletHit },
	//	{"DestroyEnemy", m_SaveData.KumiteHard.DestroyEnemy },
	//	{"HitBalloon", m_SaveData.KumiteHard.HitBalloon },
	//	{"Score", m_SaveData.KumiteHard.Score },
	//};

	// �Z�[�u�f�[�^���㏑���o��
	ofstream ofs("asset\\json\\savedata.json");
	ofs << setw(4) << j << endl;
}

/*******************************************************************************
*	����
*******************************************************************************/
SaveData SavedataManager::Create()
{
	SaveData data;

	//data.TargetBreaking.NormalTargetBreak = 0;
	//data.TargetBreaking.RareTargetBreak = 0;
	//data.TargetBreaking.Score = 0;

	//data.KumiteNormal.BulletHit = 0;
	//data.KumiteNormal.ChargeBulletHit = 0;
	//data.KumiteNormal.DestroyEnemy = 0;
	//data.KumiteNormal.HitBalloon = 0;
	//data.KumiteNormal.Score = 0;

	//data.KumiteHard.BulletHit = 0;
	//data.KumiteHard.ChargeBulletHit = 0;
	//data.KumiteHard.DestroyEnemy = 0;
	//data.KumiteHard.HitBalloon = 0;
	//data.KumiteHard.Score = 0;

	return data;
}

/*******************************************************************************
*	������
*******************************************************************************/
void SavedataManager::Clear()
{
	m_SaveData = Create();
}
