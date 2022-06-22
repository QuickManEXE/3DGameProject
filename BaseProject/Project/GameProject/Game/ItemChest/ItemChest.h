#pragma once

#include"../StaticMeshObject.h"
#include"../../Singleton/Singleton.h"

//��{�}�`���f���I�u�W�F�N�g�N���X
class ItemChest : public StaticMeshObject {
private:

	enum {
		container,
		lid,
		max_count,
	};

	bool m_is_open;//�J�����ǂ���

	CModelObj m_model[max_count];

	Transform m_chest_transform[max_count];

	CMatrix m_chest_matrix[max_count];

	CollisionTask m_Col;

	float m_rad;

	CCapsule m_capsule;

	CVector3D target_rotation;

	float time;
public:
	ItemChest(Transform _transform);
	void Update()override;
	void Render()override;
	void CollisionCheck(CollisionTask* _task);
	void OpenChest();

};