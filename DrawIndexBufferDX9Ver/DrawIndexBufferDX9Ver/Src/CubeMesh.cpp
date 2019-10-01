#include "Engine/Graphics.h"
#include "CubeMesh.h"

/**
* @brief �����̃��b�V���C���f�b�N�X�o�b�t�@�쐬�֐�@n
* �����̃��b�V�����쐬���邽�߂̃C���f�b�N�X�o�b�t�@�̍쐬�����Ă��̌��ʂ�Ԃ�
* @return �쐬������(������true)
*/
bool CreateIndexBuffer();

/**
* @brief �����̃��b�V�����_�o�b�t�@�쐬�֐�@n
* �����̃��b�V�����쐬���邽�߂̒��_�o�b�t�@�̍쐬�����Ă��̌��ʂ�Ԃ�
* @return �쐬������(������true)
*/
bool CreateVertexBuffer();

// �����̃��b�V��
static MeshModel g_CubeMesh;

bool InitCubeMesh()
{
	if (CreateVertexBuffer() == false)
	{
		return false;
	}

	if (CreateIndexBuffer() == false)
	{
		return false;
	}

	return true;
}

bool CreateVertexBuffer()
{
	// ���_�f�[�^
	MeshVertex cube_model[] =
	{
		{ -1.00000, 1.00000, -1.00000, 0xff0000ff },
		{ 1.00000, 1.00000, -1.00000, 0xff0000ff },
		{ 1.00000, -1.00000, -1.00000, 0xff0000ff },
		{ -1.00000, -1.00000, -1.00000, 0xff0000ff },
		{ -1.00000, 1.00000, 1.00000, 0xff0000ff },
		{ 1.00000, 1.00000,  1.00000, 0xff0000ff },
		{ 1.00000, 1.00000, -1.00000, 0xff0000ff },
		{ -1.00000, 1.00000, -1.00000, 0xff0000ff },
		{ 1.00000, -1.00000, -1.00000, 0xff0000ff },
		{ -1.00000, -1.00000, -1.00000, 0xff0000ff },
		{ 1.00000, -1.00000, 1.00000, 0xff0000ff },
		{ -1.00000, -1.00000, 1.00000, 0xff0000ff },
	};

	// ���_�o�b�t�@��
	g_CubeMesh.m_VertexNum = 8;
	g_CubeMesh.m_VertexList = new MeshVertex[g_CubeMesh.m_VertexNum];

	for (int i = 0; i < g_CubeMesh.m_VertexNum; i++)
	{
		g_CubeMesh.m_VertexList[i] = cube_model[i];
	}

	// ���_�o�b�t�@�쐬
	if (CreateVertexBuffer(&g_CubeMesh, g_CubeMesh.m_VertexNum * sizeof(MeshVertex)) == false)
	{
		return false;
	}

	// ���_�o�b�t�@�Ƀf�[�^���R�s�[����
	MeshVertex* list;
	if (SUCCEEDED(g_CubeMesh.m_VertexBuffer->Lock(
		// Lock����̈�̃I�t�Z�b�g�l
		0,
		// Lock����̈�̃T�C�Y(0�Ȃ�S��)
		0, 
		// ���b�N���ꂽ�|�C���^�ϐ����󂯎��ϐ�
		(void**)&list, 
		// ���b�N�̎��
		0
	)))
	{
		for (int i = 0; i < g_CubeMesh.m_VertexNum; i++)
		{
			list[i] = g_CubeMesh.m_VertexList[i];
		}

		g_CubeMesh.m_VertexBuffer->Unlock();

		return true;
	}

	return false;
}

bool CreateIndexBuffer()
{
	// �C���f�b�N�X�o�b�t�@
	g_CubeMesh.m_FaceNum = 12;
	g_CubeMesh.m_IndexList = new WORD[(3 * g_CubeMesh.m_FaceNum)];

	WORD index_list[][3] =
	{
		{ 0, 1, 2 },
		{ 0, 2, 3 },
		{ 3, 2, 4 },
		{ 3, 4, 5 },
		{ 5, 4, 6 },
		{ 5, 6, 7 },
		{ 7, 6, 1 },
		{ 7, 1, 0 },
		{ 1, 6, 4 },
		{ 1, 4, 2 },
		{ 1, 6, 4 },
		{ 7, 0, 3 },
		{ 7, 3, 5 },
	};

	for (int i = 0; i < g_CubeMesh.m_FaceNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			g_CubeMesh.m_IndexList[i * 3 + j] = index_list[i][j];
		}
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	CreateIndexBuffer(&g_CubeMesh, g_CubeMesh.m_FaceNum * sizeof(WORD) * 3, sizeof(WORD));

	// �C���f�b�N�X�o�b�t�@�ɃR�s�[����
	unsigned short* index_buffer;
	if (SUCCEEDED(g_CubeMesh.m_IndexBuffer->Lock(
		// Lock����̈�̃I�t�Z�b�g�l
		0,
		// Lock����̈�̃T�C�Y(0�Ȃ�S��)
		0,
		// ���b�N���ꂽ�|�C���^�ϐ����󂯎��ϐ�
		(void**)& index_buffer,
		// ���b�N�̎��
		0
	)))
	{

		for (int i = 0; i < g_CubeMesh.m_FaceNum; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				index_buffer[i * 3 + j] = g_CubeMesh.m_IndexList[i * 3 + j];
			}
		}

		g_CubeMesh.m_IndexBuffer->Unlock();

		return true;
	}

	return false;
}

void DrawCubeMesh()
{
	DrawMesh(&g_CubeMesh, MESH_FVF, sizeof(MeshVertex));
}

void ReleaseCubeMesh()
{
	if (g_CubeMesh.m_IndexList != nullptr)
	{
		delete[](g_CubeMesh.m_IndexList);
	}

	if (g_CubeMesh.m_IndexBuffer != nullptr)
	{
		g_CubeMesh.m_IndexBuffer->Release();
		g_CubeMesh.m_IndexBuffer = nullptr;
	}

	if (g_CubeMesh.m_VertexList != nullptr)
	{
		delete[] (g_CubeMesh.m_VertexList);
	}

	if (g_CubeMesh.m_VertexBuffer != nullptr)
	{
		g_CubeMesh.m_VertexBuffer->Release();
		g_CubeMesh.m_VertexBuffer = nullptr;
	}
}
