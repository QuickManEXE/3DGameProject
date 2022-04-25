#pragma once
#include"Effect2D.h"
#include"Effect3D.h"


class EffectCollection : public Base{
private:
	float m_time;//エフェクト表示時間
	//2D
public:
	//3D
	//血が散るエフェクト
	static void BloodSplash3D(const CVector3D& pos,CVector3D* p_parent_pos = nullptr);
	//地点から土が散るエフェクト
	static void RockSplash3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
	//地点に衝撃波が広がるエフェクト
	static void ShockWave3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
	//地点に煙が広がるエフェクト
	static void SmokeWave3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
	//地点に閃光が広がるエフェクト
	static void LightSplash3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
};