#include "ItemChest.h"
#include"../../StarterAsset.h"
#include"../CharacterBase/CharacterBase.h"

ItemChest::ItemChest(Transform _transform) :
	StaticMeshObject(UpdatePriority::eUp_Field, "ItemChest", StarterAsset::Cylinder, RenderPriority::eRd_Field)
{
	m_Transform = _transform;

	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&ItemChest::CollisionCheck, CollisionPriority::eCol_Field, "ItemChest");

	m_rad = 1.5f;

	m_model[container] = COPY_RESOURCE(StarterAsset::Cube, CModelObj);
	m_model[lid] = COPY_RESOURCE(StarterAsset::Cone, CModelObj);

	m_chest_transform[container].position = CVector3D(0, 0, 0);
	m_chest_transform[lid].position = CVector3D(0, 3, 0);

	//m_capsule.Set(m_chest_transform[container].position + CVector3D(0, 1, 0), m_chest_transform[container].position + CVector3D(0, 1, 0));
	m_capsule.SetRad(m_rad);
}

void ItemChest::Update()
{
	CMatrix parent_matrix =
		CMatrix::MTranselate(m_Transform.position) *
		CMatrix::MRotation(m_Transform.rotation);


	m_chest_matrix[container] =
		parent_matrix * CMatrix::MTranselate(m_chest_transform[container].position) *
		CMatrix::MRotation(m_chest_transform[container].rotation);// *CMatrix::MScale(CVector3D(2, 0.5, 4));
	m_chest_matrix[lid] =
		parent_matrix * CMatrix::MTranselate(m_chest_transform[lid].position) *
		CMatrix::MRotation(m_chest_transform[lid].rotation);// *CMatrix::MScale(CVector3D(2, 0.5, 4));


	m_capsule.Set(m_chest_matrix[container].GetPosition(), m_chest_matrix[container].GetPosition() + CVector3D(0, 1, 0));

}

void ItemChest::Render()
{
	for (int i = 0; i < max_count; i++) {
		CMatrix m = m_chest_matrix[i];
		m_model[i].Render(m);
	}

	//Utility::DrawCapsule(m_capsule.m_start, m_capsule.m_end, m_capsule.m_rad, CColorRGBA(0, 0, 1, 1));
}

void ItemChest::CollisionCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		CVector3D dir;
		float dist;

		if (CCollision::CollisionCapsule(m_capsule.m_start, m_capsule.m_end, m_capsule.m_rad,
			c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, &dist, &dir)) {

			float s = m_capsule.m_rad + c->m_Capsule.m_rad - dist;

			c->m_Transform.position += dir * s;

		}

	}
}
