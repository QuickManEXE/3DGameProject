#ifndef INCLUDE_TRANSFORM_GAMEPARTS
#define INCLUDE_TRANSFORM_GAMEPARTS

#include "../stdafx.h"

class Transform {
public:
	//座標
	CVector3D position;
	//回転
	CVector3D rotation;
	//大きさ
	CVector3D scale;
	//移動ベクトル
	CVector3D m_pos_vec;

	CVector3D m_rot_vec;

	CVector3D m_front;

	CVector3D m_left;

	CVector3D m_up;
	//
	CVector3D m_dir;
public:
	Transform(const CVector3D& position = CVector3D::zero,
			  const CVector3D& rotation = CVector3D::zero, 
		      const CVector3D& size = CVector3D::one);
	/*
	ローカル座標で移動する
	*/
	void Translate(const CVector3D& _translation);
	/*
	ワールド座標で移動する
	*/
	void Rotate(float x, float y, float z);
	void Rotate(const CVector3D& _rotation);
	void LookAt(const Transform& _transform);
};
#endif // !INCLUDE_TRANSFORM_GAMEPARTS