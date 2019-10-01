#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Shader.h"

// �ÓI���C�u����
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;	// ���_�p�o�b�t�@
LPD3DXEFFECT g_pShader;						// �V�F�[�_�f�[�^
D3DXHANDLE g_Tech;							// �e�N�j�b�N�ւ̃n���h��
D3DXHANDLE g_Matrix;						// �V�F�[�_�̃O���[�o���ϐ��p�n���h��(�}�g���N�X)
D3DXHANDLE g_Color;							// �V�F�[�_�̃O���[�o���ϐ��p�n���h��(�J���[)
LPDIRECT3DVERTEXDECLARATION9 g_pVertexDecl;	// ���_�f�N�����[�V�����ւ̃|�C���^

extern LPDIRECT3DDEVICE9 g_pD3DDevice;
LPDIRECT3DTEXTURE9 m_pTexture;
float m_Width;
float m_Height;
LPCWSTR file_name = TEXT("BackGround.png");
BOOL InitShader(void)
{
	// ���_�̃t�H�[�}�b�g�錾
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

	// ���_�̍쐬
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
	// ���_��ҏW���邽�߂Ƀ��b�N
	g_pVertexBuffer->Lock(0, 0, (void**)&tmp_vertex, 0);

	// ���_���W�̕ύX
	tmp_vertex[0].m_Pos.x = 0.0f; tmp_vertex[0].m_Pos.y = 30.0f; tmp_vertex[0].m_Pos.z = 100.0f;
	tmp_vertex[1].m_Pos.x = 30.0f; tmp_vertex[1].m_Pos.y = 0.0f; tmp_vertex[1].m_Pos.z = 100.0f;
	tmp_vertex[2].m_Pos.x = -30.0f; tmp_vertex[2].m_Pos.y = 0.0f; tmp_vertex[2].m_Pos.z = 100.0f;
	tmp_vertex[3].m_Pos.x = -60.0f; tmp_vertex[3].m_Pos.y = 30.0f; tmp_vertex[3].m_Pos.z = 100.0f;

	// ���_�̃��b�N������
	g_pVertexBuffer->Unlock();


	// �e�N�X�`���ǂݍ���
	if (FAILED( D3DXCreateTextureFromFile(g_pD3DDevice, file_name, &m_pTexture))){
		return false;
	}
	else {
		// �e�N�X�`���T�C�Y�̎擾
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
	// �V�F�[�_�̓ǂݍ���
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

	// �e�N�j�b�N�̃n���h���擾
	g_Tech = g_pShader->GetTechniqueByName("StandardDraw");

	// �O���[�o���ϐ��̃n���h���擾
	g_Matrix = g_pShader->GetParameterByName(NULL, "g_Matrix");
	g_Color = g_pShader->GetParameterByName(NULL, "g_Color");

	return true;
}

void DrawShader(D3DXMATRIX projection_matrix)
{
	// ���_�̗�������
	g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX));

	// �F�ύX
	D3DXVECTOR4 color;
	color.x = 1.0f;
	color.y = 1.0f;
	color.z = 1.0f;
	color.w = 1.0f;

	// ���_�t�H�[�}�b�g�̐ݒ�
	g_pD3DDevice->SetVertexDeclaration(g_pVertexDecl);

	// �e�N�j�b�N�̐ݒ�
	g_pShader->SetTechnique(g_Tech);
	// CustomVertex�̎w��
	g_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	// �p�����[�^(�O���[�o���ϐ��̐ݒ�)
	g_pShader->SetMatrix(g_Matrix, &projection_matrix);
	g_pShader->SetVector(g_Color, &color);
	// 
	g_pShader->CommitChanges();

	// �V�F�[�_�N��
	g_pShader->Begin(0, 0);
	// �p�X�N��
	g_pShader->BeginPass(0);

	g_pD3DDevice->SetTexture(0, m_pTexture);

	// �|���S���`��
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// �p�X�I��
	g_pShader->EndPass();
	// �V�F�[�_�I��
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