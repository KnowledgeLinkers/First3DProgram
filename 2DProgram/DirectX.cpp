#include <Windows.h>
#include "DirectX.h"

// グローバル変数
LPDIRECT3D9 g_pD3DInterface;	// DirectXインターフェース
D3DPRESENT_PARAMETERS *g_pD3DPresentParam;
LPDIRECT3DDEVICE9 g_pD3DDevice;

bool InitDirectX(HWND window_handle)
{
	// インターフェース作成
	g_pD3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3DInterface == NULL)
	{
		// 作成失敗
		return false;
	}

	g_pD3DPresentParam = new D3DPRESENT_PARAMETERS;
	if (g_pD3DPresentParam == NULL)
	{
		return false;
	}
	ZeroMemory(g_pD3DPresentParam, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファの数 => 1
	g_pD3DPresentParam->BackBufferCount = 1;
	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
	g_pD3DPresentParam->BackBufferFormat = D3DFMT_UNKNOWN;
	// ウィンドウモード設定 => 定数で切り替え
	g_pD3DPresentParam->Windowed = true;

	// スワップエフェクト設定 => ディスプレイドライバ依存
	// スワップエフェクト => バックバッファとフロントバッファへの切り替え方法
	g_pD3DPresentParam->SwapEffect = D3DSWAPEFFECT_DISCARD;

	// DirectDeviceの作成
	if (FAILED(g_pD3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window_handle,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		g_pD3DPresentParam,
		&g_pD3DDevice)))
	{
		return false;
	}

	// ビューポートパラメータ
	D3DVIEWPORT9 view_port;

	// ビューポートの左上座標
	view_port.X = 0;
	view_port.Y = 0;
	// ビューポートの幅
	view_port.Width = g_pD3DPresentParam->BackBufferWidth;
	// ビューポートの高さ
	view_port.Height = g_pD3DPresentParam->BackBufferHeight;
	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// ビューポート設定
	if (FAILED(g_pD3DDevice->SetViewport(&view_port)))
	{
		return false;
	}

	return true;
}

void Draw()
{
	g_pD3DDevice->Clear(0L,
		NULL,
		D3DCLEAR_TARGET,			// 初期化するバッファの種類
		D3DCOLOR_ARGB(255, 0, 0, 0),// フレームバッファの初期化色
		1.0f,						// Zバッファの初期値
		0);							// ステンシルバッファの初期値

	g_pD3DDevice->BeginScene();

	// 三角形を描画 start
	VERTEX v[3];

	v[0].pos.x = 200.0f; v[0].pos.y = 120.0f; v[0].pos.z = 0.0f;
	v[1].pos.x = 240.0f; v[1].pos.y = 200.0f; v[1].pos.z = 0.0f;
	v[2].pos.x = 160.0f; v[2].pos.y = 200.0f; v[2].pos.z = 0.0f;

	v[0].w = v[1].w = v[2].w = 1.0f;

	v[0].color = D3DCOLOR_ARGB(255, 255, 0, 0);
	v[1].color = D3DCOLOR_ARGB(255, 255, 0, 0);
	v[2].color = D3DCOLOR_ARGB(255, 255, 0, 0);

	// 頂点構造の指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, v, sizeof(VERTEX));

	// 三角形を描画 end

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}