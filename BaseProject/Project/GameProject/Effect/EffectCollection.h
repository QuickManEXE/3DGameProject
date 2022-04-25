#pragma once
#include"Effect2D.h"
#include"Effect3D.h"


class EffectCollection : public Base{
private:
	float m_time;//�G�t�F�N�g�\������
	//2D
public:
	//3D
	//�����U��G�t�F�N�g
	static void BloodSplash3D(const CVector3D& pos,CVector3D* p_parent_pos = nullptr);
	//�n�_����y���U��G�t�F�N�g
	static void RockSplash3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
	//�n�_�ɏՌ��g���L����G�t�F�N�g
	static void ShockWave3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
	//�n�_�ɉ����L����G�t�F�N�g
	static void SmokeWave3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
	//�n�_�ɑM�����L����G�t�F�N�g
	static void LightSplash3D(const CVector3D& pos, CVector3D* p_parent_pos = nullptr);
};