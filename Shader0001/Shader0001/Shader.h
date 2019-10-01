#ifndef SHADER_H_
#define SHADER_H_

BOOL InitShader(void);

void DrawShader(D3DXMATRIX projection_matrix);

void DeleteShader(void);

// \‘¢‘ÌéŒ¾
typedef struct _VERTEX
{
	D3DXVECTOR3 m_Pos;
}VERTEX;


#endif