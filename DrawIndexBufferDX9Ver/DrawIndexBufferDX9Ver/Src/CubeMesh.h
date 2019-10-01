/**
* @file CubeMesh.h
* @brief 立方体メッシュ関連ファイル
*/
#ifndef CUBE_MESH_H_
#define CUBE_MESH_H_

/**
* @brief 立方体メッシュ初期化関数@n
* 立方体メッシュの初期化を行い、その結果を返す
* @return 初期化結果(成功はtrue)
*/
bool InitCubeMesh();

/**
* @brief 立方体メッシュ描画関数@n
* 立方体メッシュを描画する
*/
void DrawCubeMesh();

/**
* @brief 立方体メッシュ解放関数@n
* 立方体メッシュデータを解放する
*/
void ReleaseCubeMesh();

#endif
