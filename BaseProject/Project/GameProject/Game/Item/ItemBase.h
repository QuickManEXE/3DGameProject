#pragma once
#include"../StaticMeshObject.h"
#include"../../StarterAsset.h"

//基本図形モデルオブジェクトクラス
class ItemBase : public StaticMeshObject {
private:

	int m_item_id;

	CollisionTask m_Col;

	float m_rad;
public:
	ItemBase(int _item_id,const Transform& transform, const char* model_name = StarterAsset::Cube);
	//virtual void Render();
	void CollisionCheck(CollisionTask* _task);
};