#pragma once
#include "stdafx.h"


namespace Utility2 {

	//安藤作
	//移動キー入力取得
	CVector3D GetInputKeyDir();
	//マウスの移動で視点移動


	//ローカルベクトル表示
	void DebugLocalDir(CVector3D pos, CVector3D rot);
	//targetがviewpointの視野角と範囲に入っているか判定
	bool IsViewInside(const CVector3D& view_pos, const CVector3D& view_rot,
		const CVector3D& target_pos, float view_ang, float view_length);
	//ランダムでプラスかマイナスを決めて返す
	int IsPlusMinus();
	//視野範囲の簡易表示
	void DrawViewPoint(CVector3D view_point, CVector3D view_rot, float view_ang, float view_length);
	//3Dベクトルをランダムで返す
	const CVector3D& RandVec3D(float min, float max);
	//当たり判定

	//円同士の判定BからAへの力を返す
	bool CollisionSphere(const CVector3D& a_center, float a_rad,
		const CVector3D& b_center, float b_rad, CVector3D* push_back);


	bool CollisionCapsuleSphere(const CVector3D& c_top, const CVector3D& c_bottom, float c_rad,
		const CVector3D& s_center, float s_rad, CVector3D* push_back);


	//カプセル同士の判定BからAへの力を返す
	bool CollisionCapsule(const CVector3D& a_top, const CVector3D& a_bottom, float a_rad,
		const CVector3D& b_top, const CVector3D& b_bottom, float b_rad, CVector3D* push_back);
}