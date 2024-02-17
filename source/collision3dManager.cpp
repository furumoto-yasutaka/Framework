/*******************************************************************************
*
*	タイトル：	3Dコリジョン管理用静的クラス
*	ファイル：	collision3dManager.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "collision3dManager.h"
#include "renderer.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

void Collision3DManager::Init(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 size)
{
	if (level > m_TREE_MAX_LEVEL) { return; }

	m_Min = min;
	m_Size = size;
	m_Level = level;

	// 各分割レベルでの空間数を計算
	m_Pow[0] = 1;
	for (int i = 1; i < m_TREE_MAX_LEVEL + 2; i++)
	{
		m_Pow[i] = m_Pow[i - 1] * 8;
	}

	// 空間数を計算
	m_CellNum = (m_Pow[m_Level + 1] - 1) / 7;
	// 空間オブジェクトを生成
	m_TreeCellList = DBG_NEW TreeCell<Collider3D>[m_CellNum];
	// 最小空間の空間サイズを計算
	m_Unit = m_Size / (float)(1 << m_Level);

#ifdef _DEBUG
	InitDebug();
#endif
}

void Collision3DManager::Uninit()
{
	for (auto data : m_TreeCellDataList)
	{
		delete data;
	}

	delete[] m_TreeCellList;

#ifdef _DEBUG
	UninitDebug();
#endif
}

void Collision3DManager::Update()
{
	if (!m_TreeCellList) { return; }

	// コリジョンの所属空間を更新
	UpdateCells();

	// 衝突判定
#ifdef _DEBUG
	m_CollisionCount = 0;
#endif
	list<Collider3D*> stack;
	CheckCollision(0, stack);

	// 衝突コールバックを実行
	CheckCallback();
}

void Collision3DManager::Draw()
{
#ifdef _DEBUG
	DrawDebug();
#endif
}

/*******************************************************************************
*	コリジョンコンポーネントを空間に登録
*******************************************************************************/
TreeCellData<Collider3D>* Collision3DManager::RegistCollision(Collider3D* collider)
{
	// 空間登録用オブジェクト生成・登録
	TreeCellData<Collider3D>* data = DBG_NEW TreeCellData<Collider3D>(collider);
	m_TreeCellDataList.push_back(data);

	// 空間オブジェクトにコンポーネントを登録する
	RegistCellData(data);

	return data;
}

/*******************************************************************************
*	コリジョンコンポーネントを空間から削除
*******************************************************************************/
void Collision3DManager::RemoveTreeCellData(TreeCellData<Collider3D>* data)
{
	m_TreeCellDataList.remove(data);

	for (auto itr = m_TreeCellDataList.begin(); itr != m_TreeCellDataList.end(); itr++)
	{
		(*itr)->m_Object->RemoveRemainElement(data->m_Object);
	}

	delete data;
}

/*******************************************************************************
*	空間のパラメータを変更
*******************************************************************************/
void Collision3DManager::ResetSpace(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 size)
{
	// 空間オブジェクトが一度消えるため一旦空間から離脱させる
	for (auto itr = m_TreeCellDataList.begin(); itr != m_TreeCellDataList.end(); itr++)
	{
		(*itr)->Remove();
	}

	m_Level = level;
	m_Min = min;
	m_Size = size;

	// 空間数を計算
	m_CellNum = (m_Pow[m_Level + 1] - 1) / 7;
	// 空間オブジェクトを生成
	delete[] m_TreeCellList;
	m_TreeCellList = DBG_NEW TreeCell<Collider3D>[m_CellNum];
	// 最小空間の空間サイズを計算
	m_Unit = m_Size / ((float)(1 << m_Level));

	// 再登録処理
	for (auto itr = m_TreeCellDataList.begin(); itr != m_TreeCellDataList.end(); itr++)
	{
		RegistCellData((*itr));
	}

#ifdef _DEBUG
	UninitDebug();
	InitDebug();
#endif
}

/*******************************************************************************
*	要素を空間に登録
*******************************************************************************/
void Collision3DManager::RegistCellData(TreeCellData<Collider3D>* data)
{
	D3DXVECTOR3 min = data->m_Object->GetMortonMinPos();
	D3DXVECTOR3 max = data->m_Object->GetMortonMaxPos();
	DWORD index = GetMortonNumber(min, max);
	if (index < m_CellNum)
	{
		m_TreeCellList[index].Push(data);
	}
}

/*******************************************************************************
*	コリジョンコンポーネントの所属空間の更新
*******************************************************************************/
void Collision3DManager::UpdateCells()
{
	for (auto itr = m_TreeCellDataList.begin(); itr != m_TreeCellDataList.end(); itr++)
	{
		// 一旦所属空間から離脱
		(*itr)->Remove();

		// 登録処理
		RegistCellData((*itr));
	}
}

/*******************************************************************************
*	衝突判定(再帰処理)
*******************************************************************************/
void Collision3DManager::CheckCollision(DWORD index, list<Collider3D*>& stack)
{
	TreeCellData<Collider3D>* baseData = m_TreeCellList[index].GetDataList();
	TreeCellData<Collider3D>* pertnerData;
	list<Collider3D*> objList;
	int objCount = 0;

	// 空間内のオブジェクトと衝突判定をする
	while (baseData)
	{
		if (baseData->m_Object->GetIsActive())
		{		
			// 同じ空間のオブジェクトと判定
			pertnerData = baseData->m_Next;
			while (pertnerData)
			{
				if (pertnerData->m_Object->GetIsActive())
				{
					baseData->m_Object->Collision(pertnerData->m_Object);
				}
				pertnerData = pertnerData->m_Next;
			}

			// 親にあたる空間のオブジェクトを格納したスタックと判定
			for (auto pertnerDataItr = stack.begin(); pertnerDataItr != stack.end(); pertnerDataItr++)
			{
				if (!(*pertnerDataItr)->GetIsActive()) { continue; }

				baseData->m_Object->Collision((*pertnerDataItr));
			}
		}

		// スタック登録用のリストに追加
		objList.push_back(baseData->m_Object);
		// 次のデータへ進める
		baseData = baseData->m_Next;
		objCount++;
	}
	// スタックに連結
	stack.splice(stack.end(), objList);

	// 子空間の衝突判定を進める
	DWORD ChildIndex = index * 8 + 1;
	if (ChildIndex < m_CellNum)
	{
		for (int i = 0; i < 8; i++, ChildIndex++)
		{
			CheckCollision(ChildIndex, stack);
		}
	}

	// スタックからこの空間のオブジェクトを外す
	auto itr = stack.rbegin();
	for (int i = 0; i < objCount; i++)
	{
		itr++;
	}
	stack.erase(itr.base(), stack.end());
}

/*******************************************************************************
*	衝突コールバック処理
*******************************************************************************/
void Collision3DManager::CheckCallback()
{
	for (TreeCellData<Collider3D>* c : m_TreeCellDataList)
	{
		c->m_Object->CheckCallback();
	}
}

/*******************************************************************************
*	所属空間をモートン番号で求める
*******************************************************************************/
DWORD Collision3DManager::GetMortonNumber(D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	// 最大レベルにおける所属空間を計算
	DWORD minPoint = GetCellIndex(Min);
	DWORD maxPoint = GetCellIndex(Max);

	DWORD xor_value = minPoint ^ maxPoint;
	int shift = 0;
	int belongLevel = m_Level;

	while (xor_value != 0)
	{
		// 空間をまたいでいるので所属レベルを下げる
		belongLevel--;
		shift += 3;

		xor_value >>= 3;
	}

	// 親空間から見て何番目の空間(0～7)にいるか求める
	int morton = maxPoint >> shift;

	// 親空間のモートン番号を求める
	int add = 1;
	for (int i = 0; i < belongLevel; i++)
	{
		add *= 8;
	}
	add = (add - 1) / 7;

	// 上記2つを足し合わせたものが所属している空間のモートン番号
	return morton + add;
}

/*******************************************************************************
*	座標→線形8分木要素番号変換
*******************************************************************************/
DWORD Collision3DManager::GetCellIndex(D3DXVECTOR3 Pos)
{
	BYTE x, y, z;

	Pos -= m_Min;
	x = (BYTE)(Pos.x / m_Unit.x);
	y = (BYTE)(Pos.y / m_Unit.y);
	z = (BYTE)(Pos.z / m_Unit.z);

	DWORD result = BitSeparate(x) | (BitSeparate(y) << 1) | (BitSeparate(z) << 2);

	return result;
}

/*******************************************************************************
*	座標→線形8分木要素番号変換
*******************************************************************************/
DWORD Collision3DManager::BitSeparate(BYTE n)
{
	DWORD s = n;
	s = (s | (s << 8)) & 0x0000f00f;
	s = (s | (s << 4)) & 0x000c30c3;
	s = (s | (s << 2)) & 0x00249249;
	return s;
}

/*******************************************************************************
*	デバッグ用関数
*******************************************************************************/
#ifdef _DEBUG
void Collision3DManager::InitDebug()
{
	//-------------------
	// バッファ設定
	//-------------------
	m_Line = 1;
	m_Line <<= m_Level;
	m_Line += 1;

	m_VertexNum = (m_Line * m_Line) * 3 * 2;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VERTEX_3D) * m_VertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 軸に平行な線の数 * 3軸 * 2頂点
	VERTEX_3D* vertex = DBG_NEW VERTEX_3D[m_VertexNum];
	D3D11_SUBRESOURCE_DATA sd;
	CreateVertex(vertex);
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	// バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	delete[] vertex;

	// シェーダー設定
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "debugLineVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "debugLinePS.cso");
}

void Collision3DManager::UninitDebug()
{
	m_VertexBuffer->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_VertexLayout->Release();
}

void Collision3DManager::DrawDebug()
{
	if (!DebugManager::m_IsSpaceDivisionFrame) { return; }

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(m_VertexNum, 0);
}

void Collision3DManager::CreateVertex(VERTEX_3D* Vertex)
{
	int index = 0;
	float rate = 1.0f / (m_Line - 1);
	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR d = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
	D3DXVECTOR2 c = D3DXVECTOR2(0.0f, 0.0f);

	// X軸
	CreateVertex_X(Vertex, index, rate, n, d, c);
	// Y軸
	CreateVertex_Y(Vertex, index, rate, n, d, c);
	// Z軸
	CreateVertex_Z(Vertex, index, rate, n, d, c);
}

void Collision3DManager::CreateVertex_X(VERTEX_3D* vertex, int& index, float rate,
	D3DXVECTOR3 n, D3DXCOLOR d, D3DXVECTOR2 c)
{
	for (int i = 0; i < m_Line; i++)
	{
		for (int j = 0; j < m_Line; j++)
		{
			vertex[index].Position = m_Min + D3DXVECTOR3(0.0f, i * rate * m_Size.y, j * rate * m_Size.z);
			vertex[index].Normal = n;
			vertex[index].Diffuse = d;
			vertex[index].TexCoord = c;

			index++;

			vertex[index].Position = m_Min + D3DXVECTOR3(m_Size.x, i * rate * m_Size.y, j * rate * m_Size.z);
			vertex[index].Normal = n;
			vertex[index].Diffuse = d;
			vertex[index].TexCoord = c;

			index++;
		}
	}
}

void Collision3DManager::CreateVertex_Y(VERTEX_3D* vertex, int& index, float rate,
	D3DXVECTOR3 n, D3DXCOLOR d, D3DXVECTOR2 c)
{
	for (int i = 0; i < m_Line; i++)
	{
		for (int j = 0; j < m_Line; j++)
		{
			vertex[index].Position = m_Min + D3DXVECTOR3(i * rate * m_Size.x, 0.0f, j * rate * m_Size.z);
			vertex[index].Normal = n;
			vertex[index].Diffuse = d;
			vertex[index].TexCoord = c;

			index++;

			vertex[index].Position = m_Min + D3DXVECTOR3(i * rate * m_Size.x, m_Size.y, j * rate * m_Size.z);
			vertex[index].Normal = n;
			vertex[index].Diffuse = d;
			vertex[index].TexCoord = c;

			index++;
		}
	}
}

void Collision3DManager::CreateVertex_Z(VERTEX_3D* vertex, int& index, float rate,
	D3DXVECTOR3 n, D3DXCOLOR d, D3DXVECTOR2 c)
{
	for (int i = 0; i < m_Line; i++)
	{
		for (int j = 0; j < m_Line; j++)
		{
			vertex[index].Position = m_Min + D3DXVECTOR3(i * rate * m_Size.x, j * rate * m_Size.y, 0.0f);
			vertex[index].Normal = n;
			vertex[index].Diffuse = d;
			vertex[index].TexCoord = c;

			index++;

			vertex[index].Position = m_Min + D3DXVECTOR3(i * rate * m_Size.x, j * rate * m_Size.y, m_Size.z);
			vertex[index].Normal = n;
			vertex[index].Diffuse = d;
			vertex[index].TexCoord = c;

			index++;
		}
	}
}
#endif
