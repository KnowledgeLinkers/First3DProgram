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

void Transform(float i)
{
	D3DXMATRIX matProj, matView;

	//ビュー座標変換用の行列算出 start
	D3DXVECTOR3 camera_pos(0.0f , 0.0f, -10.0f ); // カメラ位置
	D3DXVECTOR3 eye_pos(0.0f, 0.0f, 0.0f);		// 注視点
	D3DXVECTOR3 up_vector(0.0f, 1.0f, 0.0f);	// カメラの向き

	D3DXMatrixIdentity(&matView);
	D3DXMatrixLookAtLH(&matView, 
		&camera_pos,				// カメラ座標
		&eye_pos,					// 注視点座標
		&up_vector);				// カメラの上の向きのベクトル
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	//ビュー座標変換用の行列算出 end

	//射影座標変換用の行列算出 start\Repos
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);
	float aspect = (float)vp.Width / (float)vp.Height;

	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DXToRadian(45),	// 画角
		aspect,				// アスペクト比
		0.1f,				// near
		500.0f);			// far
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	//射影座標変換用の行列算出 end
}

void Draw()
{
	g_pD3DDevice->Clear(0L,
		NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 0),
		1.0f,	// Zバッファの初期値
		0);		// ステンシルバッファの初期値

	g_pD3DDevice->BeginScene();

	//ライティングを無効にする。
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 三角形を描画
	// ポリゴンのローカル座標の位置を指定 start
	VERTEX v[4];
	v[0].color = D3DCOLOR_ARGB(255, 255, 255, 255);
	v[1].color = D3DCOLOR_ARGB(255, 255, 255, 255);
	v[2].color = D3DCOLOR_ARGB(255, 255, 255, 255);
	v[3].color = D3DCOLOR_ARGB(255, 255, 255, 255);

	v[0].pos.x = 0.0f; v[0].pos.y = 3.5f; v[0].pos.z = 0.0f;
	v[1].pos.x = 6.0f; v[1].pos.y = 0.0f; v[1].pos.z = 0.0f;
	v[2].pos.x = -6.0f; v[2].pos.y = 0.0f; v[2].pos.z = 0.0f;
	v[3].pos.x = 0.0f; v[3].pos.y = -3.5f; v[3].pos.z = 0.0f;


	IDirect3DVertexBuffer9 *pVB;
	IDirect3DIndexBuffer9 *d3dib;
	BYTE *bBuf;

	g_pD3DDevice->BeginScene();

	g_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX) * 4, 0, D3DFVF_XYZRHW, D3DPOOL_MANAGED, &pVB, NULL);
	pVB->Lock(0, 0, (VOID**)bBuf, 0);
	memcpy(pVB, v, sizeof(D3DFVF_XYZRHW) * 4);
	pVB->Unlock();


	WORD index[] = { 0 , 1 , 2 , 2 , 0 , 3 };
	g_pD3DDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &d3dib, NULL);
	d3dib->Lock(0, 0, (VOID**)bBuf, 0);
	memcpy(bBuf, index, sizeof(WORD) * 6);
	d3dib->Unlock();


	// ポリゴンのローカル座標の位置を指定 end

	//ワールド座標変換用の行列の算出 start
	D3DXMATRIX mat_world, mat_trans, mat_rot, mat_rotx, mat_roty, mat_rotz, mat_scale;
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixIdentity(&mat_rot);
	D3DXMatrixIdentity(&mat_trans);
	D3DXMatrixIdentity(&mat_scale);

	// 移動
	D3DXMatrixTranslation(&mat_trans, 0.0f, 0.0f, 10.0f);

	// 回転
	D3DXMatrixRotationX(&mat_rotx, D3DXToRadian(0.0f));
	D3DXMatrixRotationY(&mat_roty, D3DXToRadian(0.0f));
	D3DXMatrixRotationZ(&mat_rotz, D3DXToRadian(0.0f));

	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_roty);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotx);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotz);

	// 拡大
	D3DXMatrixScaling(&mat_scale, 1.0f, 1.0f, 1.0f);

	// 掛け合わせ(拡縮 * 回転 * 移動)
	mat_world *= mat_scale * mat_rot * mat_trans;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat_world);
	//ワールド座標変換用の行列の算出 end

	g_pD3DDevice->SetStreamSource(0, pVB, 0, sizeof(VERTEX));
	g_pD3DDevice->SetIndices(d3dib);


	g_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}