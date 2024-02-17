/*******************************************************************************
*
*	�^�C�g���F	3D�R���W�����Ǘ��p�ÓI�N���X
*	�t�@�C���F	collision3dManager.cpp
*	�쐬�ҁF	�Ö{ �ח�
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

	// �e�������x���ł̋�Ԑ����v�Z
	m_Pow[0] = 1;
	for (int i = 1; i < m_TREE_MAX_LEVEL + 2; i++)
	{
		m_Pow[i] = m_Pow[i - 1] * 8;
	}

	// ��Ԑ����v�Z
	m_CellNum = (m_Pow[m_Level + 1] - 1) / 7;
	// ��ԃI�u�W�F�N�g�𐶐�
	m_TreeCellList = DBG_NEW TreeCell<Collider3D>[m_CellNum];
	// �ŏ���Ԃ̋�ԃT�C�Y���v�Z
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

	// �R���W�����̏�����Ԃ��X�V
	UpdateCells();

	// �Փ˔���
#ifdef _DEBUG
	m_CollisionCount = 0;
#endif
	list<Collider3D*> stack;
	CheckCollision(0, stack);

	// �Փ˃R�[���o�b�N�����s
	CheckCallback();
}

void Collision3DManager::Draw()
{
#ifdef _DEBUG
	DrawDebug();
#endif
}

/*******************************************************************************
*	�R���W�����R���|�[�l���g����Ԃɓo�^
*******************************************************************************/
TreeCellData<Collider3D>* Collision3DManager::RegistCollision(Collider3D* collider)
{
	// ��ԓo�^�p�I�u�W�F�N�g�����E�o�^
	TreeCellData<Collider3D>* data = DBG_NEW TreeCellData<Collider3D>(collider);
	m_TreeCellDataList.push_back(data);

	// ��ԃI�u�W�F�N�g�ɃR���|�[�l���g��o�^����
	RegistCellData(data);

	return data;
}

/*******************************************************************************
*	�R���W�����R���|�[�l���g����Ԃ���폜
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
*	��Ԃ̃p�����[�^��ύX
*******************************************************************************/
void Collision3DManager::ResetSpace(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 size)
{
	// ��ԃI�u�W�F�N�g����x�����邽�߈�U��Ԃ��痣�E������
	for (auto itr = m_TreeCellDataList.begin(); itr != m_TreeCellDataList.end(); itr++)
	{
		(*itr)->Remove();
	}

	m_Level = level;
	m_Min = min;
	m_Size = size;

	// ��Ԑ����v�Z
	m_CellNum = (m_Pow[m_Level + 1] - 1) / 7;
	// ��ԃI�u�W�F�N�g�𐶐�
	delete[] m_TreeCellList;
	m_TreeCellList = DBG_NEW TreeCell<Collider3D>[m_CellNum];
	// �ŏ���Ԃ̋�ԃT�C�Y���v�Z
	m_Unit = m_Size / ((float)(1 << m_Level));

	// �ēo�^����
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
*	�v�f����Ԃɓo�^
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
*	�R���W�����R���|�[�l���g�̏�����Ԃ̍X�V
*******************************************************************************/
void Collision3DManager::UpdateCells()
{
	for (auto itr = m_TreeCellDataList.begin(); itr != m_TreeCellDataList.end(); itr++)
	{
		// ��U������Ԃ��痣�E
		(*itr)->Remove();

		// �o�^����
		RegistCellData((*itr));
	}
}

/*******************************************************************************
*	�Փ˔���(�ċA����)
*******************************************************************************/
void Collision3DManager::CheckCollision(DWORD index, list<Collider3D*>& stack)
{
	TreeCellData<Collider3D>* baseData = m_TreeCellList[index].GetDataList();
	TreeCellData<Collider3D>* pertnerData;
	list<Collider3D*> objList;
	int objCount = 0;

	// ��ԓ��̃I�u�W�F�N�g�ƏՓ˔��������
	while (baseData)
	{
		if (baseData->m_Object->GetIsActive())
		{		
			// ������Ԃ̃I�u�W�F�N�g�Ɣ���
			pertnerData = baseData->m_Next;
			while (pertnerData)
			{
				if (pertnerData->m_Object->GetIsActive())
				{
					baseData->m_Object->Collision(pertnerData->m_Object);
				}
				pertnerData = pertnerData->m_Next;
			}

			// �e�ɂ������Ԃ̃I�u�W�F�N�g���i�[�����X�^�b�N�Ɣ���
			for (auto pertnerDataItr = stack.begin(); pertnerDataItr != stack.end(); pertnerDataItr++)
			{
				if (!(*pertnerDataItr)->GetIsActive()) { continue; }

				baseData->m_Object->Collision((*pertnerDataItr));
			}
		}

		// �X�^�b�N�o�^�p�̃��X�g�ɒǉ�
		objList.push_back(baseData->m_Object);
		// ���̃f�[�^�֐i�߂�
		baseData = baseData->m_Next;
		objCount++;
	}
	// �X�^�b�N�ɘA��
	stack.splice(stack.end(), objList);

	// �q��Ԃ̏Փ˔����i�߂�
	DWORD ChildIndex = index * 8 + 1;
	if (ChildIndex < m_CellNum)
	{
		for (int i = 0; i < 8; i++, ChildIndex++)
		{
			CheckCollision(ChildIndex, stack);
		}
	}

	// �X�^�b�N���炱�̋�Ԃ̃I�u�W�F�N�g���O��
	auto itr = stack.rbegin();
	for (int i = 0; i < objCount; i++)
	{
		itr++;
	}
	stack.erase(itr.base(), stack.end());
}

/*******************************************************************************
*	�Փ˃R�[���o�b�N����
*******************************************************************************/
void Collision3DManager::CheckCallback()
{
	for (TreeCellData<Collider3D>* c : m_TreeCellDataList)
	{
		c->m_Object->CheckCallback();
	}
}

/*******************************************************************************
*	������Ԃ����[�g���ԍ��ŋ��߂�
*******************************************************************************/
DWORD Collision3DManager::GetMortonNumber(D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	// �ő僌�x���ɂ����鏊����Ԃ��v�Z
	DWORD minPoint = GetCellIndex(Min);
	DWORD maxPoint = GetCellIndex(Max);

	DWORD xor_value = minPoint ^ maxPoint;
	int shift = 0;
	int belongLevel = m_Level;

	while (xor_value != 0)
	{
		// ��Ԃ��܂����ł���̂ŏ������x����������
		belongLevel--;
		shift += 3;

		xor_value >>= 3;
	}

	// �e��Ԃ��猩�ĉ��Ԗڂ̋��(0�`7)�ɂ��邩���߂�
	int morton = maxPoint >> shift;

	// �e��Ԃ̃��[�g���ԍ������߂�
	int add = 1;
	for (int i = 0; i < belongLevel; i++)
	{
		add *= 8;
	}
	add = (add - 1) / 7;

	// ��L2�𑫂����킹�����̂��������Ă����Ԃ̃��[�g���ԍ�
	return morton + add;
}

/*******************************************************************************
*	���W�����`8���ؗv�f�ԍ��ϊ�
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
*	���W�����`8���ؗv�f�ԍ��ϊ�
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
*	�f�o�b�O�p�֐�
*******************************************************************************/
#ifdef _DEBUG
void Collision3DManager::InitDebug()
{
	//-------------------
	// �o�b�t�@�ݒ�
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

	// ���ɕ��s�Ȑ��̐� * 3�� * 2���_
	VERTEX_3D* vertex = DBG_NEW VERTEX_3D[m_VertexNum];
	D3D11_SUBRESOURCE_DATA sd;
	CreateVertex(vertex);
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	// �o�b�t�@����
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	delete[] vertex;

	// �V�F�[�_�[�ݒ�
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

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���[���h�}�g���N�X�ݒ�
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(m_VertexNum, 0);
}

void Collision3DManager::CreateVertex(VERTEX_3D* Vertex)
{
	int index = 0;
	float rate = 1.0f / (m_Line - 1);
	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR d = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
	D3DXVECTOR2 c = D3DXVECTOR2(0.0f, 0.0f);

	// X��
	CreateVertex_X(Vertex, index, rate, n, d, c);
	// Y��
	CreateVertex_Y(Vertex, index, rate, n, d, c);
	// Z��
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
