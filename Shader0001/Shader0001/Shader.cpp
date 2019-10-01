#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Shader.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;	// 頂点用バッファ
LPD3DXEFFECT g_pShader;						// シェーダデータ
D3DXHANDLE g_Tech;							// テクニックへのハンドル
D3DXHANDLE g_Matrix;						// シェーダのグローバル変数用ハンドル(マトリクス)
D3DXHANDLE g_Color;							// シェーダのグローバル変数用ハンドル(カラー)
LPDIRECT3DVERTEXDECLARATION9 g_pVertexDecl;	// 頂点デクラレーションへのポインタ

extern LPDIRECT3DDEVICE9 g_pD3DDevice;
LPDIRECT3DTEXTURE9 m_pTexture;
float m_Width;
float m_Height;
LPCWSTR file_name = TEXT("BackGround.png");
BOOL InitShader(void)
{
	// 頂点のフォーマット宣言
	D3DVERTEXELEMENT9 vertex_elements[] = 
	{
		{
			0,
			0,
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},
		D3DDECL_END()
	};


	g_pD3DDevice->CreateVertexDeclaration(vertex_elements, &g_pVertexDecl);

	// 頂点の作成
	if (FAILED(g_pD3DDevice->CreateVertexBuffer(
			4 * sizeof(VERTEX),
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&g_pVertexBuffer,
			NULL)))
	{
		return false;
	}

	VERTEX *tmp_vertex;
	// 頂点を編集するためにロック
	g_pVertexBuffer->Lock(0, 0, (void**)&tmp_vertex, 0);

	// 頂点座標の変更
	tmp_vertex[0].m_Pos.x = 0.0f; tmp_vertex[0].m_Pos.y = 30.0f; tmp_vertex[0].m_Pos.z = 100.0f;
	tmp_vertex[1].m_Pos.x = 30.0f; tmp_vertex[1].m_Pos.y = 0.0f; tmp_vertex[1].m_Pos.z = 100.0f;
	tmp_vertex[2].m_Pos.x = -30.0f; tmp_vertex[2].m_Pos.y = 0.0f; tmp_vertex[2].m_Pos.z = 100.0f;
	tmp_vertex[3].m_Pos.x = -60.0f; tmp_vertex[3].m_Pos.y = 30.0f; tmp_vertex[3].m_Pos.z = 100.0f;

	// 頂点のロックを解除
	g_pVertexBuffer->Unlock();


	// テクスチャ読み込み
	if (FAILED( D3DXCreateTextureFromFile(g_pD3DDevice, file_name, &m_pTexture))){
		return false;
	}
	else {
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;

		if (FAILED(m_pTexture->GetLevelDesc(0, &desc)))
		{
			m_pTexture->Release();
			m_pTexture = NULL;
			return false;
		}
		m_Width = (float)desc.Width;
		m_Height = (float)desc.Height;
	}
	// シェーダの読み込み
	LPD3DXBUFFER error = NULL;
	D3DXCreateEffectFromFile(g_pD3DDevice,
							TEXT("Shader.hlsl"),
							0,
							0,
							D3DXSHADER_DEBUG,
							0,
							&g_pShader,
							&error);
	if (g_pShader == NULL)
	{
		return false;
	}

	// テクニックのハンドル取得
	g_Tech = g_pShader->GetTechniqueByName("StandardDraw");

	// グローバル変数のハンドル取得
	g_Matrix = g_pShader->GetParameterByName(NULL, "g_Matrix");
	g_Color = g_pShader->GetParameterByName(NULL, "g_Color");

	return true;
}

void DrawShader(D3DXMATRIX projection_matrix)
{
	// 頂点の流し込み
	g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX));

	// 色変更
	D3DXVECTOR4 color;
	color.x = 1.0f;
	color.y = 1.0f;
	color.z = 1.0f;
	color.w = 1.0f;

	// 頂点フォーマットの設定
	g_pD3DDevice->SetVertexDeclaration(g_pVertexDecl);

	// テクニックの設定
	g_pShader->SetTechnique(g_Tech);
	// CustomVertexの指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	// パラメータ(グローバル変数の設定)
	g_pShader->SetMatrix(g_Matrix, &projection_matrix);
	g_pShader->SetVector(g_Color, &color);
	// 
	g_pShader->CommitChanges();

	// シェーダ起動
	g_pShader->Begin(0, 0);
	// パス起動
	g_pShader->BeginPass(0);

	g_pD3DDevice->SetTexture(0, m_pTexture);

	// ポリゴン描画
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// パス終了
	g_pShader->EndPass();
	// シェーダ終了
	g_pShader->End();
}

void DeleteShader(void)
{
	if (g_pVertexBuffer != NULL)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	if (g_pVertexDecl != NULL)
	{
		g_pVertexDecl->Release();
		g_pVertexDecl = NULL;
	}

	if (g_pShader != NULL)
	{
		g_pShader->Release();
		g_pShader = NULL;
	}
}