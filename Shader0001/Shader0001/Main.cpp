#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Shader.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define SHADER_ON

// グローバル変数
LPDIRECT3D9 g_pD3DInterface;				// DirectXインターフェース
D3DPRESENT_PARAMETERS *g_pD3DPresentParam;	// プレゼンテーションパラメータ
LPDIRECT3DDEVICE9 g_pD3DDevice;				// Deviece

// プロトタイプ関数宣言
BOOL MainLoop(void);


LRESULT CALLBACK WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),				// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,		// クラスのスタイル
		WindowProc,						// ウィンドウプロシージャ
		0,								// 補助メモリ
		0,								// 補助メモリ
		hInstance,						// このプログラムのインスタンスハンドル
		LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)),// アイコン画像
		LoadCursor(NULL, IDC_ARROW),	// カーソル画像
		NULL,							// 背景ブラシ(背景色)
		NULL,							// メニュー名
		TEXT("StencilTest"),			// クラス名									
		NULL							// 小さいアイコン
	};

	// 構造体の登録
	if (RegisterClassEx(&window_class) == 0)
	{
		return false;
	}

	// ウィンドウ作成
	hWnd = CreateWindow(
		TEXT("StencilTest"),
		TEXT("StencilTest"),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE,
		400,
		10,
		400,
		400,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == NULL)
	{
		return false;
	}

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

	// 深度バッファの有無
	g_pD3DPresentParam->EnableAutoDepthStencil = TRUE;

	// 深度バッファのフォーマット
	g_pD3DPresentParam->AutoDepthStencilFormat = D3DFMT_D24S8;

	// 多様なフラグ設定
	g_pD3DPresentParam->Flags = 0;

	// DirectDeviceの作成
	if (FAILED(g_pD3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		g_pD3DPresentParam,
		&g_pD3DDevice)))
	{
		return false;
	}

	// ビューポートの設定
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

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	InitShader();

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			MainLoop();
		}
	}

	DeleteShader();
}

BOOL MainLoop()
{
	g_pD3DDevice->Clear(0L,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 0),
		1.0f,	// Zバッファの初期値
		0);		// ステンシルバッファの初期値

	g_pD3DDevice->BeginScene();

	//ライティングを無効にする。
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX matProj, matView, matWorld;

	////射影座標変換
	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DX_PI / 4.0f,
		4.0f / 3.0f,
		0.1f,
		500.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	//ビュー座標変換
	D3DXMatrixIdentity(&matView);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	//ワールド座標変換
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);


#ifndef SHADER_ON
	g_pD3DDevice->SetFVF(D3DFVF_XYZ);
	
	// ステンシルバッファには三角形を描画
	VERTEX v[3];

	v[0].m_Pos.x = 0.0f; v[0].m_Pos.y = 3.5f; v[0].m_Pos.z = 29.0f;
	v[1].m_Pos.x = 6.0f; v[1].m_Pos.y = -2.5f; v[1].m_Pos.z = 29.0f;
	v[2].m_Pos.x = -6.0f; v[2].m_Pos.y = -2.5f; v[2].m_Pos.z = 29.0f;

	g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, v, sizeof(VERTEX));
#else
	DrawShader(matProj);
#endif
	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	return TRUE;
}
