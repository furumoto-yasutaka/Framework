/*******************************************************************************
*
*	�^�C�g���F	���؂�1��ԕ��̃N���X
*	�t�@�C���F	treeCell.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
template<class T>
class TreeCellData;

// ���؂�1��ԕ��̃N���X
template<class T>
class TreeCell
{
private:
	TreeCellData<T>* m_DataList = NULL;	// ��ԓ��̃I�u�W�F�N�g�̐擪�A�h���X

public:
	~TreeCell()
	{
		// �S�Ă̗v�f���폜����
		TreeCellData<T>* temp;
		while (m_DataList)
		{
			// �폜�̑O�Ɏ��g�̎��̗v�f��ۑ�����
			temp = m_DataList->m_Next;
			// �擪�v�f���폜
			delete m_DataList;
			// �ۑ������v�f��擪�Ƃ���
			m_DataList = temp;
		}
	}

	// �I�u�W�F�N�g��o�^
	void Push(TreeCellData<T>* data)
	{
		// �s���Ȓl�܂��͊��Ɏ��g��o�^���Ă���ꍇ�͓o�^���Ȃ�
		if (!data || data->m_BelongToCell == this) { return; }

		// �ʂ̋�Ԃɑ����Ă���ꍇ�͂��̋�Ԃ���폜
		// (�ǂ̋�Ԃɂ������Ă��Ȃ��p�^�[���͕��؂͈̔͊O����͈͓��ɖ߂�悤�ȏꍇ�ɔ���)
		if (data->m_BelongToCell != NULL)
		{
			data->Remove();
		}

		// ���݋�Ԃɕʂ̃I�u�W�F�N�g�����݂��邩
		if (m_DataList)
		{// ���݂���
			// �Q�Ƃ��������ēo�^
			data->m_Next = m_DataList;
			m_DataList->m_Prev = data;
			m_DataList = data;
		}
		else
		{// ���݂��Ȃ�
			// ����œo�^
			m_DataList = data;
		}
		data->m_BelongToCell = this;
	}

	// �I�u�W�F�N�g�̍폜�ʒm
	void OnRemove(TreeCellData<T>* data)
	{
		// �擪�v�f���폜���ꂽ�ꍇ�|�C���^���X�V����
		if (m_DataList == data && data != NULL)
		{
			m_DataList = m_DataList->m_Next;
		}
	}

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	TreeCellData<T>* GetDataList() { return m_DataList; }
	/////////////////////////////
};

// �R���W����1���̏����������f�[�^�N���X
template<class T>
class TreeCellData
{
public:
	TreeCell<T>*		m_BelongToCell = NULL;	// �����Ă�����
	T*					m_Object = NULL;		// �Ǘ��ΏۃR���|�[�l���g
	TreeCellData<T>*	m_Prev = NULL;			// �����Ă����Ԃɂ����Ď��g�̑O�̗v�f
	TreeCellData<T>*	m_Next = NULL;			// �����Ă����Ԃɂ����Ď��g�̎��̗v�f

public:
	TreeCellData(T* object)
	{
		m_Object = object;
	}
	~TreeCellData() {}

	// ��ԃI�u�W�F�N�g�̃��X�g����폜����
	// (���W�ړ��ɂ��ēo�^���ƃR���|�[�l���g�폜���ɌĂяo�����)
	void Remove()
	{
		if (m_BelongToCell != NULL)
		{
			// �폜�ʒm
			m_BelongToCell->OnRemove(this);

			// �Q�Ƃ𐮗�
			if (m_Prev) { m_Prev->m_Next = m_Next; }
			if (m_Next) { m_Next->m_Prev = m_Prev; }

			m_Prev = NULL;
			m_Next = NULL;
			
			m_BelongToCell = NULL;
		}
	}
};
