#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Shader.h"

// �ÓI���C�u����
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define SHADER_ON

// �O���[�o���ϐ�
LPDIRECT3D9 g_pD3DInterface;				// DirectX�C���^�[�t�F�[�X
D3DPRESENT_PARAMETERS *g_pD3DPresentParam;	// �v���[���e�[�V�����p�����[�^
LPDIRECT3DDEVICE9 g_pD3DDevice;				// Deviece

// �v���g�^�C�v�֐��錾
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
		sizeof(WNDCLASSEX),				// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,		// �N���X�̃X�^�C��
		WindowProc,						// �E�B���h�E�v���V�[�W��
		0,								// �⏕������
		0,								// �⏕������
		hInstance,						// ���̃v���O�����̃C���X�^���X�n���h��
		LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)),// �A�C�R���摜
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���摜
		NULL,							// �w�i�u���V(�w�i�F)
		NULL,							// ���j���[��
		TEXT("StencilTest"),			// �N���X��									
		NULL							// �������A�C�R��
	};

	// �\���̂̓o�^
	if (RegisterClassEx(&window_class) == 0)
	{
		return false;
	}

	// �E�B���h�E�쐬
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

	// �C���^�[�t�F�[�X�쐬
	g_pD3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3DInterface == NULL)
	{
		// �쐬���s
		return false;
	}

	g_pD3DPresentParam = new D3DPRESENT_PARAMETERS;
	if (g_pD3DPresentParam == NULL)
	{
		return false;
	}
	ZeroMemory(g_pD3DPresentParam, sizeof(D3DPRESENT_PARAMETERS));

	// �o�b�N�o�b�t�@�̐� => 1
	g_pD3DPresentParam->BackBufferCount = 1;
	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g => D3DFMT_UNKNOWN(�t�H�[�}�b�g��m��܂���)
	g_pD3DPresentParam->BackBufferFormat = D3DFMT_UNKNOWN;
	// �E�B���h�E���[�h�ݒ� => �萔�Ő؂�ւ�
	g_pD3DPresentParam->Windowed = true;

	// �X���b�v�G�t�F�N�g�ݒ� => �f�B�X�v���C�h���C�o�ˑ�
	// �X���b�v�G�t�F�N�g => �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�ւ̐؂�ւ����@
	g_pD3DPresentParam->SwapEffect = D3DSWAPEFFECT_DISCARD;

	// �[�x�o�b�t�@�̗L��
	g_pD3DPresentParam->EnableAutoDepthStencil = TRUE;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	g_pD3DPresentParam->AutoDepthStencilFormat = D3DFMT_D24S8;

	// ���l�ȃt���O�ݒ�
	g_pD3DPresentParam->Flags = 0;

	// DirectDevice�̍쐬
	if (FAILED(g_pD3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		g_pD3DPresentParam,
		&g_pD3DDevice)))
	{
		return false;
	}

	// �r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 view_port;					
	// �r���[�|�[�g�̍�����W
	view_port.X = 0;
	view_port.Y = 0;
	// �r���[�|�[�g�̕�
	view_port.Width = g_pD3DPresentParam->BackBufferWidth;
	// �r���[�|�[�g�̍���
	view_port.Height = g_pD3DPresentParam->BackBufferHeight;
	// �r���[�|�[�g�[�x�ݒ�
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// �r���[�|�[�g�ݒ�
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
		1.0f,	// Z�o�b�t�@�̏����l
		0);		// �X�e���V���o�b�t�@�̏����l

	g_pD3DDevice->BeginScene();

	//���C�e�B���O�𖳌��ɂ���B
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX matProj, matView, matWorld;

	////�ˉe���W�ϊ�
	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DX_PI / 4.0f,
		4.0f / 3.0f,
		0.1f,
		500.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	//�r���[���W�ϊ�
	D3DXMatrixIdentity(&matView);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	//���[���h���W�ϊ�
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);


#ifndef SHADER_ON
	g_pD3DDevice->SetFVF(D3DFVF_XYZ);
	
	// �X�e���V���o�b�t�@�ɂ͎O�p�`��`��
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
