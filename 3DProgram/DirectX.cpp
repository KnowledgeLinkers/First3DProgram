#include <Windows.h>
#include "DirectX.h"

// �O���[�o���ϐ�
LPDIRECT3D9 g_pD3DInterface;	// DirectX�C���^�[�t�F�[�X
D3DPRESENT_PARAMETERS *g_pD3DPresentParam;
LPDIRECT3DDEVICE9 g_pD3DDevice;

bool InitDirectX(HWND window_handle)
{
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

	// DirectDevice�̍쐬
	if (FAILED(g_pD3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window_handle,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		g_pD3DPresentParam,
		&g_pD3DDevice)))
	{
		return false;
	}

	// �r���[�|�[�g�p�����[�^
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

	return true;
}

void Transform(float i)
{
	D3DXMATRIX matProj, matView;

	//�r���[���W�ϊ��p�̍s��Z�o start
	D3DXVECTOR3 camera_pos(0.0f , 0.0f, -10.0f ); // �J�����ʒu
	D3DXVECTOR3 eye_pos(0.0f, 0.0f, 0.0f);		// �����_
	D3DXVECTOR3 up_vector(0.0f, 1.0f, 0.0f);	// �J�����̌���

	D3DXMatrixIdentity(&matView);
	D3DXMatrixLookAtLH(&matView, 
		&camera_pos,				// �J�������W
		&eye_pos,					// �����_���W
		&up_vector);				// �J�����̏�̌����̃x�N�g��
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	//�r���[���W�ϊ��p�̍s��Z�o end

	//�ˉe���W�ϊ��p�̍s��Z�o start\Repos
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);
	float aspect = (float)vp.Width / (float)vp.Height;

	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DXToRadian(45),	// ��p
		aspect,				// �A�X�y�N�g��
		0.1f,				// near
		500.0f);			// far
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	//�ˉe���W�ϊ��p�̍s��Z�o end
}

void Draw()
{
	g_pD3DDevice->Clear(0L,
		NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 0),
		1.0f,	// Z�o�b�t�@�̏����l
		0);		// �X�e���V���o�b�t�@�̏����l

	g_pD3DDevice->BeginScene();

	//���C�e�B���O�𖳌��ɂ���B
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �O�p�`��`��
	// �|���S���̃��[�J�����W�̈ʒu���w�� start
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


	// �|���S���̃��[�J�����W�̈ʒu���w�� end

	//���[���h���W�ϊ��p�̍s��̎Z�o start
	D3DXMATRIX mat_world, mat_trans, mat_rot, mat_rotx, mat_roty, mat_rotz, mat_scale;
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixIdentity(&mat_rot);
	D3DXMatrixIdentity(&mat_trans);
	D3DXMatrixIdentity(&mat_scale);

	// �ړ�
	D3DXMatrixTranslation(&mat_trans, 0.0f, 0.0f, 10.0f);

	// ��]
	D3DXMatrixRotationX(&mat_rotx, D3DXToRadian(0.0f));
	D3DXMatrixRotationY(&mat_roty, D3DXToRadian(0.0f));
	D3DXMatrixRotationZ(&mat_rotz, D3DXToRadian(0.0f));

	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_roty);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotx);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotz);

	// �g��
	D3DXMatrixScaling(&mat_scale, 1.0f, 1.0f, 1.0f);

	// �|�����킹(�g�k * ��] * �ړ�)
	mat_world *= mat_scale * mat_rot * mat_trans;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat_world);
	//���[���h���W�ϊ��p�̍s��̎Z�o end

	g_pD3DDevice->SetStreamSource(0, pVB, 0, sizeof(VERTEX));
	g_pD3DDevice->SetIndices(d3dib);


	g_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}