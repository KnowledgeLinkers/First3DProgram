/**
* @file Graphics.h
* @brief 描画に関する関数、定数の宣言
*/
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <Windows.h>
#include <d3d9.h>
#include "../Utility/Vec.h"

//!< メッシュ用FVF設定
#define MESH_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//!< メッシュ用頂点構造体
struct MeshVertex
{
	float X;			//!< X座標
	float Y;			//!< Y座標
	float Z;			//!< Z座標

	unsigned int Color;	//!< カラー
};

//!< メッシュ構造体
struct MeshModel
{
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;	//!< 頂点バッファインターフェース
	LPDIRECT3DINDEXBUFFER9 m_IndexBuffer;	//!< インデックスバッファインタフェース
	int m_VertexNum;						//!< 頂点数
	WORD m_FaceNum;							//!< ポリゴン数
	MeshVertex* m_VertexList;				//!< 頂点バッファ
	WORD* m_IndexList;						//!< インデックスバッファ
};

/**
* @brief Graphics機能初期化関数@n
* 描画関連を使用可能にするための初期化を行う@n
* ※Engine.cppのInitEngineで実行しているので@n
* 　使用者は自分たちで使う必要はない
* @return 初期化結果 (trueが成功)
*/
bool InitGraphics();

/**
* @brief Graphics機能終了関数@n
* 描画関連の処理の後始末を行う処理@n
* ※Engine.cppのEndEngineで実行しているので@n
* 　使用者は自分たちで使う必要はない
*/
void EndGraphics();

/**
* @brief 描画開始関数@n
* 描画開始を宣言し、バックバッファのクリアを行う
* @return 描画開始の成否 (成功はtrue)
*/
bool DrawStart();

/**
* @brief 描画終了関数@n
* 描画の終了を宣言し、バックバッファとフロントバッファを入れ替える@n
* ※この関数は必ずDrawStartの後に実行されるようにしてください。
*/
void DrawEnd();

/**
* @brief VertexBuffer作成関数@n
* 指定したサイズのVertexBufferを作成する
* @return 作成の成否 (成功はtrue)
* @param[in] mesh 作成後のバッファを格納するためのMeshModel構造体
* @param[in] buffer_size 作成するバッファのサイズ(頂点用構造体 * 頂点数)
*/
bool CreateVertexBuffer(MeshModel* mesh, int buffer_size);

/**
* @brief IndexBuffer作成関数@n
* 指定したサイズのIndexBufferを作成する
* @return 作成の成否 (成功はtrue)
* @param[in] mesh 作成後のバッファを格納するためのMeshModel構造体
* @param[in] buffer_size 作成するバッファのサイズ(ポリゴン数 * インデックスのサイズ * 3)
* @param[in] index_size 格納する１インデックスのサイズ(2 or 4バイト)
*/
bool CreateIndexBuffer(MeshModel* mesh, int buffer_size, int index_size);

/**
* @brief Mesh描画関数@n
* メッシュを描画する
* @param[in] mesh 作成後のバッファを格納するためのMeshModel構造体
* @param[in] fvf 頂点情報設定(D3DFVFの設定)
* @param[in] custom_vertex_size 頂点情報のサイズ
*/
void DrawMesh(MeshModel* mesh, unsigned int fvf, int custom_vertex_size);

/**
* @brief 座標変換設定関数@n
* ビュー座標、プロジェクション座標の変換行列を設定する
*/
void SetUpTransform();

#endif
