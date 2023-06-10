/*******************************************************************************
*
*	タイトル：	ゲームオブジェクトクラス
*	ファイル：	gameObject.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "component.h"
#include "manager.h"

#include <list>
#include <vector>
#include <typeinfo>
#include <string>

class GameObject
{
public:
	// 親オブジェクト設定時の子に対するオプション
	enum class SetParentOption
	{
		Nothing		= 0,
		KeepPos		= 1 << 0,		// 座標を維持
		KeepScale	= 1 << 1,		// 拡縮を維持
		All			= (1 << 2) - 1,
	};

private:
	list<Component*>	m_ComponentList;		// コンポーネントリスト

	GameObject*			m_Parent = NULL;		// 親オブジェクト
	list<GameObject*>	m_Child;				// 子オブジェクト

	bool				m_IsActive = true;		// アクティブフラグ
	bool				m_IsDestroy = false;	// 削除フラグ
	bool				m_IsStatic = false;		// 静的フラグ
	string				m_Name;					// オブジェクト名
	int					m_Tag;					// オブジェクトタグ

public:	
	D3DXVECTOR3			m_LocalPosition;		// 座標
	D3DXQUATERNION		m_LocalRotation;		// 回転
	D3DXVECTOR3			m_LocalScale;			// サイズ

public:
	~GameObject() {}

	void Init(string name, int tag);
	void Uninit();

	// コンポーネントの削除確認
	void CheckComponentDestroy();

	// 自身とその子オブジェクトの削除を予約
	void SetDestroy();
	// 削除処理
	void Destroy();

	// マトリクスを取得
	D3DXMATRIX GetMatrix();
	// 正面方向の単位ベクトルを取得
	D3DXVECTOR3 GetForward();
	// 上方向の単位ベクトルを取得
	D3DXVECTOR3 GetUp();
	// 右方向の単位ベクトルを取得
	D3DXVECTOR3 GetRight();

	/////////////////////////////
	//　↓↓　親子関係　↓↓　//
	
	// 自身のローカルマトリクスの原点におけるワールドマトリクス(親のワールドマトリクス)取得
	D3DXMATRIX GetLocalMatrixOrigin();
	// ワールドマトリクス取得
	D3DXMATRIX GetWorldMatrix(D3DXMATRIX matrix = Math::GetMatrixIdentity());

	// 自身のローカル座標の原点におけるワールド座標(親のワールド座標)取得
	D3DXVECTOR3 GetLocalPositionOrigin();
	// ワールド座標取得
	D3DXVECTOR3 GetWorldPosition(D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// ワールド座標設定
	void SetWorldPosition(D3DXVECTOR3 position);

	// 自身のローカルローテーションの原点におけるワールドローテーション(親のワールドローテーション)取得
	D3DXQUATERNION GetLocalRotationOrigin();
	// ワールドローテーション取得
	D3DXQUATERNION GetWorldRotation(D3DXQUATERNION rotation = Math::GetQuaternionIdentity());
	// ワールドローテーション設定
	void SetWorldRotation(D3DXQUATERNION rotation);

	// 自身のローカルスケールの原点におけるワールドスケール(親のワールドスケール)取得
	D3DXVECTOR3 GetLocalScaleOrigin();
	// ワールドスケール取得
	D3DXVECTOR3 GetWorldScale(D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	// ワールドスケール設定
	void SetWorldScale(D3DXVECTOR3 scale);
	/////////////////////////////

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	list<Component*> GetComponentAll();

	GameObject* GetRoot();

	GameObject* GetParent() { return m_Parent; }
	void SetParent(GameObject* parent,
		int removeOption = (int)SetParentOption::Nothing,
		int setOption = (int)SetParentOption::Nothing);

	list<GameObject*> GetChildList() { return m_Child; }
	GameObject* GetChild(int index);

	bool GetIsActive();
	bool GetIsActiveSelf() { return m_IsActive; }
	void SetIsActive(bool value) { m_IsActive = value; }

	bool GetIsDestroy() { return m_IsDestroy; }

	bool GetIsStatic() { return m_IsStatic; }
	void SetIsStatic(bool value);

	string GetName() { return m_Name; }
	void SetName(string newName);

	int GetTag() { return m_Tag; }
	void AddTag(int tag) { m_Tag |= tag; }
	/////////////////////////////

private:
	// 正面方向の単位ベクトルを取得
	D3DXVECTOR3 GetForwardInverse();
	// 上方向の単位ベクトルを取得
	D3DXVECTOR3 GetUpInverse();
	// 右方向の単位ベクトルを取得
	D3DXVECTOR3 GetRightInverse();

	void CheckRemoveOption(int option, GameObject* parent);
	void CheckSetOption(int option, GameObject* parent);

public:
	/*******************************************************************************
	*	コンポーネント追加
	*******************************************************************************/
	template<class T>
	T* AddComponent()
	{
		T* component = DBG_NEW T(this);

		m_ComponentList.push_back(component);

#ifdef _DEBUG
		component->InitDebug();

		if (component->GetDrawGroup() == Component::DrawGroup::Draw2D)
		{
			m_AttachDraw2dComponentCount++;
		}
#endif
		return component;
	}

	/*******************************************************************************
	*	コンポーネント取得(クラス名参照)
	*******************************************************************************/
	template<class T>
	T* GetComponent()
	{
		auto itr = find_if(m_ComponentList.begin(), m_ComponentList.end(),
			[](Component* c) { return typeid(*c) == typeid(T); });

		if (itr != m_ComponentList.end())
		{
			return reinterpret_cast<T*>((*itr));
		}
		else
		{
			return NULL;
		}
	}

	/*******************************************************************************
	*	コンポーネント全取得(クラス名参照)
	*******************************************************************************/
	template<class T>
	vector<T*> GetComponents()
	{
		vector<T*> components;
		for (Component* c : m_ComponentList)
		{
			if (typeid(*c) == typeid(T))
			{
				components.push_back(c);
			}
		}
		return components;
	}

	/////////////////////////////////
	//　↓↓　基本呼出禁止　↓↓　//
	// 子オブジェクトを追加　※基本呼出禁止
	void AddChild(GameObject* child) { m_Child.push_back(child); }
	// 子オブジェクトを削除　※基本呼出禁止
	void RemoveChild(GameObject* child) { m_Child.remove(child); }
	/////////////////////////////////

#ifdef _DEBUG
private:
	bool m_IsPreDestroy = false;
	int	m_AttachDraw2dComponentCount = 0; // Draw2Dコンポーネントのアタッチ数

	ID3D11Buffer* m_VertexBuffer = NULL;		// 頂点バッファ
	ID3D11VertexShader* m_VertexShader = NULL;	// 頂点シェーダー
	ID3D11PixelShader* m_PixelShader = NULL;	// ピクセルシェーダー
	ID3D11InputLayout* m_VertexLayout = NULL;	// 入力レイアウト

public:
	void DrawInspector();
	void InitDebug();
	void UninitDebug();
	void DrawDebugParentLine2d();
	void DrawDebugParentLine3d();
private:
	void DrawDebugParentLine();
	void CreateVertex(VERTEX_3D* vertex);
#endif
};
