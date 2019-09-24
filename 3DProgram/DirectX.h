#ifndef DIRECTX_H_
#define DIRECTX_H_

#include <d3d9.h>
#include <d3dx9.h>

// �ÓI���C�u����
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// �\���̐錾
typedef struct _VERTEX
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
}VERTEX;

bool InitDirectX(HWND window_handle);
void Transform(float i);
void Draw(void);

#endif