#pragma once
#include "GL.h"
#include "CMatrix.h"
#include "CRect.h"
#include"CInput.h"
class CCamera;
namespace Utility {
	/*!
		@brief	Degree（度）をRadianに変換
		@param	d				[in] 角度
		@retval	Radianに変換された角度
	**/

	inline float DgreeToRadian(float d) {
		return (float)(d*M_PI / 180.0f);
	}
	inline float RadianToDgree(float d) {
		return (float)(d *180.0f/ M_PI);
	}
	/*!
		@brief	角度の正規化
		@param	a				[in] 角度
		@retval	-PI〜PI
	**/

	inline float NormalizeAngle(float a) {
		if (a>M_PI) return (float)(a - M_PI * 2);
		else if (a<-M_PI) return (float)(a + M_PI * 2);
		return a;
	}
	/*!
		@brief	角度の差を取得
		@param	a			[in] 角度
		@param	b			[in] 角度
		@retval	aとbの角度の差
	**/
	inline float DiffAngle(float a, float b) {
		if(a*b < 0){
			if(fabsf(a)+fabsf(b) > M_PI) {
				if (b<0) a -= (float)M_PI * 2;
				else a+=(float)M_PI*2;
			}
		}
		return a-b;
	}
	/*!
		@brief	乱数取得
		@param	min				[in] 最低値
		@param	max				[in] 最大値
		@retval	最低値〜最大値の乱数
	**/

	template <typename T> inline T Rand(T min,T max) {
		return min + ((max-min) * rand()/ RAND_MAX);
	}
	/*!
		@brief	スクリーン座標変換→ワールド座標
		@param	out				[out] ワールド座標
		@param	spos			[in] スクリーン座標
		@param	mProj			[in] 射影行列
		@param	mView			[in] ビュー行列
		@param	viewport		[in] ビューポート矩形
		@retval	無し
	**/
	void ScreenToWorld(CVector3D *out, const CVector3D& spos, const CMatrix& mProj, const CMatrix& mView, const CRect& viewport);

	/*!
	@brief	スクリーン座標変換→ワールド座標
	@param	out				[out] ワールド座標
	@param	spos			[in] スクリーン座標
	@param	camera			[in] 使用カメラ
	@retval	無し
	**/
	void ScreenToWorld(CVector3D *out, const CVector3D& spos, const CCamera* camera);
	void ScreenToWorld(CVector3D *out, const CVector3D& spos);
	/*!
		@brief	ワールド座標→スクリーン座標変換
		@param	out				[out] スクリーン座標
		@param	wpos			[in] ワールド座標
		@param	mProj			[in] 射影行列
		@param	mView			[in] ビュー行列
		@param	viewport		[in] ビューポート矩形
		@retval	無し
	**/
	void WorldToScreen(CVector3D *out, const CVector3D& wpos, const CMatrix& mProj, const CMatrix& mView, const CRect& viewport);
	/*!
	@brief	ワールド座標→スクリーン座標変換
	@param	out				[out] スクリーン座標
	@param	wpos			[in] ワールド座標
	@param	camera			[in] 使用カメラ
	**/
	void WorldToScreen(CVector3D *out, const CVector3D& wpos, const CCamera* camera);
	void WorldToScreen(CVector3D *out, const CVector3D& wpos);
	void WorldToScreen(CVector2D *out, const CVector3D& wpos);
	/*!
	@brief	線を引く
	@param	s				[in] 線分の始点
	@param	e				[in] 線分の終点
	@param	color			[in] 色
	@retval	無し
	**/
	void DrawLine(const CVector2D &s, const CVector2D &e, const CVector4D &color);

	void DrawQuad(const CVector2D &pos, const CVector2D &size, const CVector4D &color);


	void DrawLine(const CVector3D &s, const CVector3D &e, const CVector4D &color);
	void DrawCube(const CVector3D &pos, const float size, const CVector4D &color);
	void DrawCube(const CMatrix &mat, const float size, const CVector4D &color);
	void DrawSphere(const CVector3D &pos, const float size, const CVector4D &color);
	void DrawSphere(const CMatrix &mat, const float size, const CVector4D &color);
	void DrawCircle(const CVector2D &pos, const float size, const CVector4D &color);
	void DrawCapsule(const CVector3D &s, const CVector3D &e, const float rad, const CVector4D &color);
//	void DrawCapsule(CVector2D & s, CVector2D &e, float size, CVector4D & color);//

	
	//安藤作
	//移動キー入力取得
	CVector3D GetInputKeyDir();

	//デバック用のPrint
	void DebugPrint(const char* format, ...);

	//ローカルベクトル表示
	void DebugLocalDir(CVector3D pos, CVector3D rot);
	//targetがviewpointの視野角と範囲に入っているか判定
	bool IsViewInside(const CVector3D& view_pos, const CVector3D& view_rot,
		const CVector3D& target_pos, float view_ang, float view_length);
	//ランダムでプラスかマイナスを決めて返す
	int IsPlusMinus();
	//視野範囲の簡易表示
	void DrawViewPoint(CVector3D view_point,CVector3D view_rot,float view_ang,float view_length);
	//3Dベクトルをランダムで返す
	const CVector3D& RandVec3D(float min, float max);

	//当たり判定
	
	//円同士の判定BからAへの力を返す
	bool CollisionSphere(const CVector3D& a_center, float a_rad,
		const CVector3D& b_center, float b_rad, CVector3D* push_back);

	
	bool CollisionCapsuleSphere(const CVector3D& c_top,const CVector3D& c_bottom,float c_rad,
		const CVector3D& s_center, float s_rad, CVector3D* push_back);

/*
	//カプセル同士の判定BからAへの力を返す
	bool CollisionCapsule(const CVector3D& a_top, const CVector3D& a_bottom, float a_rad,
		const CVector3D& b_top, const CVector3D& b_bottom, float b_rad, CVector3D* push_back);
		*/
}

#define RtoD(x) Utility::RadianToDgree(x)
#define DtoR(x) Utility::DgreeToRadian(x)