#include <d3d9.h>
#include <d3dx9.h>
#include "Graphics.h"
#include "Window.h"
#include "../Utility/Size.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// グローバル変数
static LPDIRECT3D9 g_D3DInterface;						// DirectGraphicsインターフェース
static LPDIRECT3DDEVICE9 g_D3DDevice;					// DirectGraphicsデバイス

// インターフェースの作成
bool CreateGraphicsInterface();

// グラフィックデバイスの作成
bool CreateGraphicsDevice(D3DPRESENT_PARAMETERS* present_param);

// ViewPortの設定
bool SetUpViewPort(D3DPRESENT_PARAMETERS* present_param);

bool InitGraphics()
{
	D3DPRESENT_PARAMETERS present_param;
	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));

	if (CreateGraphicsInterface() == false)
	{
		return false;
	}

	if (CreateGraphicsDevice(&present_param) == false)
	{
		return false;
	}

	if (SetUpViewPort(&present_param) == false)
	{
		return false;
	}

	return true;
}

void EndGraphics()
{
	g_D3DDevice->Release();
	g_D3DInterface->Release();
}

bool DrawStart()
{
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 0, 0), 1.0f, 0);

	//ライティングを無効にする。
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (D3D_OK == g_D3DDevice->BeginScene())
	{
		return true;
	}

	return false;
}

void DrawEnd()
{
	g_D3DDevice->EndScene();
	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

bool CreateVertexBuffer(MeshModel* model, int buffer_size)
{
	if (FAILED(g_D3DDevice->CreateVertexBuffer(
		// 作成するバッファのサイズ(頂点バッファの数 * 頂点構造体のサイズ)
		buffer_size,
		// 使用方法
		0,
		// FVF設定(SetFVF設定できるので0でも可)
		0,
		// メモリの指定
		D3DPOOL_MANAGED,
		// 生成したVertexBufferの格納先の指定
		&model->m_VertexBuffer,
		// nullptr固定
		nullptr)))
	{
		return false;
	}

	return true;
}

bool CreateIndexBuffer(MeshModel* model, int buffer_size, int index_size)
{
	_D3DFORMAT format = _D3DFORMAT::D3DFMT_INDEX16;

	if (index_size == 4)
	{
		format = _D3DFORMAT::D3DFMT_INDEX32;
	}

	if (FAILED(g_D3DDevice->CreateIndexBuffer(
		// インデックスバッファのサイズ
		buffer_size,
		// 使用方法
		D3DUSAGE_WRITEONLY,
		// インデックスバッファのフォーマット
		format,
		// メモリの指定
		D3DPOOL_MANAGED,
		// 生成したIndexBufferの格納先の指定
		&model->m_IndexBuffer,
		// nullptr固定
		nullptr)))
	{
		return false;
	}

	return true;
}

bool CreateGraphicsInterface()
{
	// インターフェース作成
	g_D3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3DInterface == NULL)
	{
		// 作成失敗
		return false;
	}

	// 作成成功
	return true;
}

bool CreateGraphicsDevice(D3DPRESENT_PARAMETERS* present_param)
{
	// バックバッファの数 => 1
	present_param->BackBufferCount = 1;
	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
	present_param->BackBufferFormat = D3DFMT_UNKNOWN;
	// ウィンドウモード設定 => 定数で切り替え
	present_param->Windowed = true;

	// スワップエフェクト => バックバッファとフロントバッファへの切り替え方法
	present_param->SwapEffect = D3DSWAPEFFECT_DISCARD;

	// ZバッファON
	present_param->EnableAutoDepthStencil = TRUE;
	present_param->AutoDepthStencilFormat = D3DFMT_D24S8;

	// DirectDeviceの作成
	if (FAILED(g_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		present_param,
		&g_D3DDevice)))
	{
		return false;
	}

	return true;
}

bool SetUpViewPort(D3DPRESENT_PARAMETERS* present_param)
{
	// ビューポートパラメータ
	D3DVIEWPORT9 view_port;

	// ビューポートの左上座標
	view_port.X = 0;
	view_port.Y = 0;
	// ビューポートの幅
	view_port.Width = present_param->BackBufferWidth;
	// ビューポートの高さ
	view_port.Height = present_param->BackBufferHeight;
	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// ビューポート設定
	if (FAILED(g_D3DDevice->SetViewport(&view_port)))
	{
		return false;
	}

	return true;
}

void DrawMesh(MeshModel* mesh, unsigned int fvf, int custom_vertex_size)
{
	D3DXMATRIX matrix;

	D3DXMatrixIdentity(&matrix);

	g_D3DDevice->SetTransform(D3DTS_WORLD, &matrix);

	// FVF設定(XYZ座標、カラー)
	g_D3DDevice->SetFVF(MESH_FVF);
	//// 頂点描画
	// 頂点バッファの登録
	g_D3DDevice->SetStreamSource(0, mesh->m_VertexBuffer, 0, custom_vertex_size);
	// インデックスバッファの登録
	g_D3DDevice->SetIndices(mesh->m_IndexBuffer);
	// 描画
	g_D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mesh->m_VertexNum, 0, mesh->m_FaceNum);
}

void SetUpTransform()
{
	// 
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;

	D3DVIEWPORT9 vp;
	g_D3DDevice->GetViewport(&vp);
	float aspect = (float)vp.Width / (float)vp.Height;

	D3DXVECTOR3 camera_pos(10.0f, 5.0f, -10.0f); // カメラ位置
	D3DXVECTOR3 eye_pos(0.0f, 0.0f, 0.0f);		// 注視点
	D3DXVECTOR3 up_vector(0.0f, 1.0f, 0.0f);	// カメラの向き
	D3DXMatrixLookAtLH(&matView,
		&camera_pos,				// カメラ座標
		&eye_pos,					// 注視点座標
		&up_vector);				// カメラの上の向きのベクトル

	g_D3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45), aspect, 1.0f, 1000.0f);

	g_D3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}
