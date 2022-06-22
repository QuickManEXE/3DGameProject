#pragma once
#include"../StaticMeshObject.h"
#include"../../StarterAsset.h"

//��{�}�`���f���I�u�W�F�N�g�N���X
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