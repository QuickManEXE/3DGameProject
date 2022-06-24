#pragma once
#include"../StaticMeshObject.h"
#include"../../StarterAsset.h"
#include"../Player/Player.h"

//基本アイテムオブジェクトクラス
class ItemBase : public StaticMeshObject {
private:

	int m_item_id;

	CollisionTask m_Col;

	float m_rad;
public:
	ItemBase(int _item_id,const Transform& transform, const char* model_name = StarterAsset::Cube);
	//virtual void Render();
	void CollisionCheck(CollisionTask* _task);
	virtual void PickUpFunc(Player* _player);
};