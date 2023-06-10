/*******************************************************************************
*
*	�^�C�g���F	���f���N���X
*	�t�@�C���F	model.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "model.h"
#include "textureContainer.h"

#include <stdio.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

Model::Model(const char* path, float scale)
{
	strcpy(m_Path, path);
	m_Scale = scale;
	Load();
}

Model::~Model()
{
	Unload();
}

void Model::Draw()
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		ID3D11ShaderResourceView* resource = m_SubsetArray[i].Material.Tex->GetResource();
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}
}

/*******************************************************************************
*	���[�h
*******************************************************************************/
void Model::Load()
{
	MODEL model;
	LoadObj(m_Path, &model);

	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = DBG_NEW SUBSET[model.SubsetNum];
		m_SubsetNum = model.SubsetNum;

		for (unsigned int i = 0; i < model.SubsetNum; i++)
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material = model.SubsetArray[i].Material;
		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;
}

/*******************************************************************************
*	�A�����[�h
*******************************************************************************/
void Model::Unload()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	delete[] m_SubsetArray;
}

/*******************************************************************************
*	obj�t�@�C���ǂݍ���
*******************************************************************************/
void Model::LoadObj(const char* fileName, MODEL* model)
{
	char dir[MAX_PATH];
	strcpy(dir, fileName);
	PathRemoveFileSpec(dir);

	D3DXVECTOR3* positionArray;
	D3DXVECTOR3* normalArray;
	D3DXVECTOR2* texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL* materialArray = NULL;
	unsigned int	materialNum = 0;

	char str[256] = "";
	char* s;
	char c;

	FILE* file;
	file = fopen(fileName, "rt");
	assert(file);

	// �v�f���J�E���g
	while (true)
	{
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0) { break; }

		if (strcmp(str, "v") == 0)
		{
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				(void)fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			// �l�p�͎O�p�ɕ���
			if (in == 4) { in = 6; }

			indexNum += in;
		}
	}

	// �������m��
	positionArray = DBG_NEW D3DXVECTOR3[positionNum];
	normalArray = DBG_NEW D3DXVECTOR3[normalNum];
	texcoordArray = DBG_NEW D3DXVECTOR2[texcoordNum];

	model->VertexArray = DBG_NEW VERTEX_3D[vertexNum];
	model->VertexNum = vertexNum;

	model->IndexArray = DBG_NEW unsigned int[indexNum];
	model->IndexNum = indexNum;

	model->SubsetArray = DBG_NEW SUBSET[subsetNum];
	model->SubsetNum = subsetNum;


	// �v�f�Ǎ�
	D3DXVECTOR3* position = positionArray;
	D3DXVECTOR3* normal = normalArray;
	D3DXVECTOR2* texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;


	fseek(file, 0, SEEK_SET);

	while (true)
	{
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "mtllib") == 0)
		{
			// �}�e���A���t�@�C��
			(void)fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			// �I�u�W�F�N�g��
			(void)fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			// ���_���W
			(void)fscanf(file, "%f", &position->x);
			(void)fscanf(file, "%f", &position->y);
			(void)fscanf(file, "%f", &position->z);
			position++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			// �@��
			(void)fscanf(file, "%f", &normal->x);
			(void)fscanf(file, "%f", &normal->y);
			(void)fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			// �e�N�X�`�����W
			(void)fscanf(file, "%f", &texcoord->x);
			(void)fscanf(file, "%f", &texcoord->y);
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			// �}�e���A��
			(void)fscanf(file, "%s", str);

			if (sc != 0)
			{
				model->SubsetArray[sc - 1].IndexNum = ic - model->SubsetArray[sc - 1].StartIndex;
			}

			model->SubsetArray[sc].StartIndex = ic;


			for (unsigned int i = 0; i < materialNum; i++)
			{
				if (strcmp(str, materialArray[i].Name) == 0)
				{
					model->SubsetArray[sc].Material = materialArray[i];
					strcpy(model->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;
		}
		else if (strcmp(str, "f") == 0)
		{
			// ��
			in = 0;

			do
			{
				(void)fscanf(file, "%s", str);

				s = strtok(str, "/");
				model->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok(NULL, "/");
					model->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(NULL, "/");
				model->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				model->VertexArray[vc].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			// �l�p�͎O�p�ɕ���
			if (in == 4)
			{
				model->IndexArray[ic] = vc - 4;
				ic++;
				model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}

	if (sc != 0)
	{
		model->SubsetArray[sc - 1].IndexNum = ic - model->SubsetArray[sc - 1].StartIndex;
	}

	fclose(file);

	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

/*******************************************************************************
*	mtl�t�@�C���ǂݍ���
*******************************************************************************/
void Model::LoadMaterial(const char* fileName, MODEL_MATERIAL** materialArray, unsigned int* materialNum)
{
	char dir[MAX_PATH];
	strcpy(dir, fileName);
	PathRemoveFileSpec(dir);

	char str[256] = "";

	FILE* file;
	file = fopen(fileName, "rt");
	assert(file);

	MODEL_MATERIAL* matArray;
	unsigned int matNum = 0;

	// �v�f���J�E���g
	while (true)
	{
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0) { break; }

		if (strcmp(str, "newmtl") == 0) { matNum++; }
	}

	// �������m��
	matArray = DBG_NEW MODEL_MATERIAL[matNum];

	// �v�f�Ǎ�
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0) { break; }

		if (strcmp(str, "newmtl") == 0)
		{
			//�}�e���A����
			mc++;
			matArray[mc].Tex = TextureContainer::GetTexture_InName(TextureContainer::GetModelTexInitial() + "1");

			(void)fscanf(file, "%s", matArray[mc].Name);

			matArray[mc].Material.Emission.r = 0;
			matArray[mc].Material.Emission.g = 0;
			matArray[mc].Material.Emission.b = 0;
			matArray[mc].Material.Emission.a = 0;
		}
		else if (strcmp(str, "Ka") == 0)
		{
			//�A���r�G���g
			(void)fscanf(file, "%f", &matArray[mc].Material.Ambient.r);
			(void)fscanf(file, "%f", &matArray[mc].Material.Ambient.g);
			(void)fscanf(file, "%f", &matArray[mc].Material.Ambient.b);
			matArray[mc].Material.Ambient.a = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//�f�B�t���[�Y
			(void)fscanf(file, "%f", &matArray[mc].Material.Diffuse.r);
			(void)fscanf(file, "%f", &matArray[mc].Material.Diffuse.g);
			(void)fscanf(file, "%f", &matArray[mc].Material.Diffuse.b);
			matArray[mc].Material.Diffuse.a = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			//�X�y�L����
			(void)fscanf(file, "%f", &matArray[mc].Material.Specular.r);
			(void)fscanf(file, "%f", &matArray[mc].Material.Specular.g);
			(void)fscanf(file, "%f", &matArray[mc].Material.Specular.b);
			matArray[mc].Material.Specular.a = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//�X�y�L�������x
			(void)fscanf(file, "%f", &matArray[mc].Material.Shininess);
		}
		else if (strcmp(str, "d") == 0)
		{
			//�A���t�@
			(void)fscanf(file, "%f", &matArray[mc].Material.Diffuse.a);
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//�e�N�X�`��
			(void)fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			matArray[mc].Tex = TextureContainer::AddModelTexture(path);
		}
	}

	fclose(file);

	*materialArray = matArray;
	*materialNum = matNum;
}
