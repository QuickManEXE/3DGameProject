#pragma once
#include "CVector.h"

class CLine {
public:
	union {
		CVector3D m_start;
		struct {
			float m_start_x;
			float m_start_y;
			float m_start_z;
		};
	};
	union {
		CVector3D m_end;
		struct {
			float m_end_x;
			float m_end_y;
			float m_end_z;
		};
	};
public:
	CLine();
	CLine(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z);
	CLine(const CVector3D& s_pos, const CVector3D& e_pos, float rad);
	void Set(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z);
	void Set(const CVector3D& s_pos, const CVector3D& e_pos);
};

class CRect {
public:
	float m_left;
	float m_top;
	float m_right;
	float m_bottom;
	union {
		struct {
			float m_x;
			float m_y;
			float m_width;
			float m_height;
		};
		struct {
			CVector2D m_pos;
			CVector2D m_size;
		};
	};
	CRect();

	/**
	* @fn
	* コンストラクタ（矩形設定）
	* @brief 矩形を設定する
	* @param l	[in]　左
	* @param t	[in]　上
	* @param r	[in]　右
	* @param b	[in]　下
	*/
	CRect(float l, float t, float r, float b);
	/**
	* @fn
	* コンストラクタ（矩形設定）
	* @brief 位置と大きさで矩形を設定する
	* @param pos	[in]　位置
	* @param size	[in]　大きさ
	*/
	CRect(const CVector2D& pos, const CVector2D& size);
	CRect(const CRect& rect);

	CRect operator=(CRect& rect);
	/**
	* @fn
	* 矩形を設定する
	* @brief 矩形を設定する
	* @param l	[in]　左
	* @param t	[in]　上
	* @param r	[in]　右
	* @param b	[in]　下
	*/
	void Set(float l, float t, float r, float b);
	/**
	* @fn
	* 矩形を設定する
	* @brief 位置と大きさで矩形を設定する
	* @param pos	[in]　位置
	* @param size	[in]　大きさ
	**/
	void Set(const CVector2D& pos, const CVector2D& size);

	/**
	* @fn
	* 矩形の大きさを取得する
	* @brief 矩形の大きさを取得する
	* @return 矩形のサイズ
	**/
	CVector2D GetSize() { return m_size; }

	/**
	* @fn
	* 矩形の最小位置を取得する
	* @brief 矩形の最小位置を取得する
	* @return 左上の座標
	**/	
	CVector2D GetMin() { return CVector2D(m_left, m_top); }

	/**
	* @fn
	* 矩形の最大位置を取得する
	* @brief 矩形の最大位置を取得する	* @return 右下の座標
	**/
	CVector2D GetMax() { return CVector2D(m_top, m_bottom); }

};

class CSphere {
public:
	union {
		CVector3D m_center;
		struct {
			float m_x;
			float m_y;
			float m_z;
		};
	};
	float m_rad;
public:
	CSphere();
	CSphere(float x, float y, float z, float rad);
	CSphere(const CVector3D& pos, float rad);
	void Set(float x, float y, float z, float rad);
	void Set(CVector3D pos, float rad);
};

class CCapsule : public CLine{
public:
	float m_rad;
public:
	CCapsule();
	CCapsule(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z, float rad);
	CCapsule(const CVector3D& s_pos, const CVector3D& e_pos, float rad);
	void SetRad(float rad);
};
