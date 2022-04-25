#pragma once
#include"NonAnimObject.h"
#include"../StarterAsset.h"

//基本図形モデルオブジェクトクラス
class Geometry : public NonAnimObject {
private:

	CollisionTask m_Col;

public:
	Geometry(const Transform& transform,const char* model_name = StarterAsset::Cube);
	//virtual void Render();
	void CollisionCheck(CollisionTask* _task);
};