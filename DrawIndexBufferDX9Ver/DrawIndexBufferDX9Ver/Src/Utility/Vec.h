/**
* @file Vec.h
* @brief 2Dの情報を保存できる構造体の宣言
*/
#ifndef VEC_H_
#define VEC_H_

//=====================================================================//
//! Vector用構造体(2D版)
//=====================================================================//
struct Vec2
{
	/** Constructor */
	Vec2()
	{
		X = 0.0f;
		Y = 0.0f;
	}

	/**
	* @brief Constructor
	* @param[in] x
	* @param[in] y
	*/
	Vec2(float x, float y)
	{
		X = x;
		Y = y;
	}

	/**
	* @brief Constructor
	* @param[in] Vec2 コピー用Vec2データ
	*/
	Vec2(const Vec2& vec)
	{
		this->X = vec.X;
		this->Y = vec.Y;
	}

	float X;	//!< X値
	float Y;	//!< Y値
};

//=====================================================================//
//! Vector用構造体(3D版)
//=====================================================================//
struct Vec3
{
	/** Constructor */
	Vec3()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	/**
	* @brief Constructor
	* @param[in] x 
	* @param[in] y
	* @param[in] z
	*/
	Vec3(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	/**
	* @brief Constructor
	* @param[in] Vec3 コピー用Vec3データ
	*/
	Vec3(const Vec3& vec)
	{
		this->X = vec.X;
		this->Y = vec.Y;
		this->Z = vec.Z;
	}

	float X;	//!< X値
	float Y;	//!< Y値
	float Z;	//!< Z値
};


#endif
