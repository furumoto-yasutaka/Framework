/*******************************************************************************
*
*	タイトル：	アプリケーション管理用静的クラス
*	ファイル：	manager.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "transition.h"

class Manager
{
public:
	static inline Scene* m_Scene = NULL;		// シーンのインスタンス
	static inline Scene* m_NextScene = NULL;	// 予約用シーンのインスタンス

protected:
	Manager() = default;
public:
	static void Init(HINSTANCE hInstance);
	static void Uninit();
	static void Update();
	static void Draw();

	// 次のシーンを予約
	template<class T>
	static void SetScene(Transition::TransitionOption info)
	{
		if (m_NextScene)
		{
			delete m_NextScene;
			m_NextScene = NULL;
		}

		m_NextScene = DBG_NEW T();

		Transition::SetTransitionOption(info);

		// シーンが存在する(exe起動直後ではない)場合
		// 遷移前トランジションを行う
		if (m_Scene)
		{
			SetTransition_In();
		}
	}

	// トランジション用ゲームオブジェクトを生成する
	static Transition* SetTransition_In();

	// シーン遷移を行うか確認
	static void CheckScene();

protected:
	static void SetInitialScene();

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	static Scene* GetScene() { return m_Scene; }
	/////////////////////////////
};
