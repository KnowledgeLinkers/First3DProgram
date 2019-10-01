#include "Engine/Graphics.h"
#include "CubeMesh.h"

/**
* @brief 立方体メッシュインデックスバッファ作成関数@n
* 立方体メッシュを作成するためのインデックスバッファの作成をしてその結果を返す
* @return 作成化結果(成功はtrue)
*/
bool CreateIndexBuffer();

/**
* @brief 立方体メッシュ頂点バッファ作成関数@n
* 立方体メッシュを作成するための頂点バッファの作成をしてその結果を返す
* @return 作成化結果(成功はtrue)
*/
bool CreateVertexBuffer();

// 立方体メッシュ
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
	// 頂点データ
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

	// 頂点バッファ数
	g_CubeMesh.m_VertexNum = 8;
	g_CubeMesh.m_VertexList = new MeshVertex[g_CubeMesh.m_VertexNum];

	for (int i = 0; i < g_CubeMesh.m_VertexNum; i++)
	{
		g_CubeMesh.m_VertexList[i] = cube_model[i];
	}

	// 頂点バッファ作成
	if (CreateVertexBuffer(&g_CubeMesh, g_CubeMesh.m_VertexNum * sizeof(MeshVertex)) == false)
	{
		return false;
	}

	// 頂点バッファにデータをコピーする
	MeshVertex* list;
	if (SUCCEEDED(g_CubeMesh.m_VertexBuffer->Lock(
		// Lockする領域のオフセット値
		0,
		// Lockする領域のサイズ(0なら全体)
		0, 
		// ロックされたポインタ変数を受け取る変数
		(void**)&list, 
		// ロックの種類
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
	// インデックスバッファ
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

	// インデックスバッファ作成
	CreateIndexBuffer(&g_CubeMesh, g_CubeMesh.m_FaceNum * sizeof(WORD) * 3, sizeof(WORD));

	// インデックスバッファにコピーする
	unsigned short* index_buffer;
	if (SUCCEEDED(g_CubeMesh.m_IndexBuffer->Lock(
		// Lockする領域のオフセット値
		0,
		// Lockする領域のサイズ(0なら全体)
		0,
		// ロックされたポインタ変数を受け取る変数
		(void**)& index_buffer,
		// ロックの種類
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
