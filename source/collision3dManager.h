/*******************************************************************************
*
*	タイトル：	3Dコリジョン管理用静的クラス
*	ファイル：	collision3dManager.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "treeCell.h"
#include "collider3d.h"

#include <list>

class Collision3DManager
{
private:
	static inline const unsigned int m_TREE_MAX_LEVEL = 6;

	static inline int m_Pow[m_TREE_MAX_LEVEL + 2];
	static inline TreeCell<Collider3D>* m_TreeCellList = NULL;
	static inline list<TreeCellData<Collider3D>*> m_TreeCellDataList;
	static inline D3DXVECTOR3 m_Min;
	static inline D3DXVECTOR3 m_Size;
	static inline D3DXVECTOR3 m_Unit;
	static inline DWORD m_CellNum = 0;
	static inline int m_Level = 0;

private:
	Collision3DManager() {}
public:
	static void Init(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 size);
	static void Uninit();
	static void Update();
	static void Draw();

	static TreeCellData<Collider3D>* RegistCollision(Collider3D* collider);
	static void RemoveTreeCellData(TreeCellData<Collider3D>* data);
	static void ResetSpace(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 size);

private:
	static void RegistCellData(TreeCellData<Collider3D>* data);
	static void UpdateCells();
	static void CheckCollision(DWORD index, list<Collider3D*>& stack);
	static void CheckCallback();

	static DWORD GetMortonNumber(D3DXVECTOR3 min, D3DXVECTOR3 max);
	// 座標→線形8分木要素番号変換関数
	static DWORD GetCellIndex(D3DXVECTOR3 pos);
	// ビット分割関数
	static DWORD BitSeparate(BYTE n);

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	static inline int GetLevel() { return m_Level; }
	static inline D3DXVECTOR3 GetMin() { return m_Min; }
	static inline D3DXVECTOR3 GetSize() { return m_Size; }
	/////////////////////////////

#ifdef _DEBUG
private:
	static inline int m_VertexNum = 0;
	static inline int m_Line = 0;
	static inline ID3D11Buffer* m_VertexBuffer = NULL;			// 頂点バッファ
	static inline ID3D11VertexShader* m_VertexShader = NULL;	// 頂点シェーダー
	static inline ID3D11PixelShader* m_PixelShader = NULL;		// ピクセルシェーダー
	static inline ID3D11InputLayout* m_VertexLayout = NULL;		// 入力レイアウト

	static inline int m_CollisionCount = 0;

private:
	static void InitDebug();
	static void UninitDebug();
	static void DrawDebug();
	static void CreateVertex(VERTEX_3D* vertex);
	static void CreateVertex_X(VERTEX_3D* vertex, int& index, float rate,
		D3DXVECTOR3 n, D3DXCOLOR d, D3DXVECTOR2 c);
	static void CreateVertex_Y(VERTEX_3D* vertex, int& index, float rate,
		D3DXVECTOR3 n, D3DXCOLOR d, D3DXVECTOR2 c);
	static void CreateVertex_Z(VERTEX_3D* vertex, int& index, float rate,
		D3DXVECTOR3 n, D3DXCOLOR d, D3DXVECTOR2 c);

public:
	static inline void AddCollisionCount() { m_CollisionCount++; }
	static inline int GetCollisionCount() { return m_CollisionCount; }
#endif
};
