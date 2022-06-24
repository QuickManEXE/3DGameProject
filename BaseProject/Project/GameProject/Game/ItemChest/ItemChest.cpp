#include "ItemChest.h"
#include"../../StarterAsset.h"
#include"../CharacterBase/CharacterBase.h"
#include"../Item/ItemBase.h"
#include"../../Effect/EffectCollection.h"
#include"../Item/HeartItem.h"
#include"../Item/AttackSpeedUpItem.h"
#include"../Item/MoveSpeedUpItem.h"

ItemChest::ItemChest(Transform _transform) :
	StaticMeshObject(UpdatePriority::eUp_Field, "ItemChest", StarterAsset::Cylinder, RenderPriority::eRd_Field)
	,m_is_open(false),m_is_item_drop(false)
{
	m_Transform = _transform;

	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&ItemChest::CollisionCheck, CollisionPriority::eCol_Field, "ItemChest");

	m_rad = 0.5f;
	m_open_rad = 1.5f;

	m_model[container] = COPY_RESOURCE("Chest_Bottom", CModelObj);
	m_model[lid] = COPY_RESOURCE("Chest_Top", CModelObj);

	m_chest_transform[container].position = CVector3D(0, 0, 0);
	m_chest_transform[lid].position = CVector3D(0, 0, 0);

	//m_capsule.Set(m_chest_transform[container].position + CVector3D(0, 1, 0), m_chest_transform[container].position + CVector3D(0, 1, 0));
	m_capsule.SetRad(m_rad);
	
	target_rotation = CVector3D(DtoR(-45), 0, 0);

	EffectCollection::SmokeWave3D(m_Transform.position, &m_Transform.position);

	CMatrix parent_matrix =
		CMatrix::MTranselate(m_Transform.position) *
		CMatrix::MRotation(m_Transform.rotation);
	m_chest_matrix[container] =
		parent_matrix * CMatrix::MTranselate(m_chest_transform[container].position) *
		CMatrix::MRotation(m_chest_transform[container].rotation);
	m_chest_matrix[lid] =
		parent_matrix * CMatrix::MTranselate(m_chest_transform[lid].position) *
		CMatrix::MRotation(m_chest_transform[lid].rotation);
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

	OpenChest();
}

void ItemChest::Render()
{
	for (int i = 0; i < max_count; i++) {
		CMatrix m = m_chest_matrix[i];
		m_model[i].Render(m);
	}

	//Utility::DrawSphere(m_Transform.position, m_rad, CColorRGBA(0.5, 0, 0, 0.5f));
	//Utility::DrawSphere(m_Transform.position, m_open_rad, CColorRGBA(0, 0.5, 0, 0.3f));
	//Utility::DrawCapsule(m_capsule.m_start, m_capsule.m_end, m_capsule.m_rad, CColorRGBA(0, 0, 1, 1));
}

void ItemChest::CollisionCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		CVector3D dir;
		float dist;


		if (CCollision::CollisionCapsuleShpere(c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, m_Transform.position, m_rad)) {


			if (CInput::GetState(0, CInput::ePush, CInput::eButton1)) {
				printf("ŠJ‚¯‚Ü‚µ‚½");

				m_is_open = true;

				time = 0;
			}

			CVector3D push_back(0, 0, 0);
			if(Utility::CollisionCapsuleSphere(c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, m_Transform.position, m_rad,&push_back)){

				c->m_Transform.position -= push_back;
			}
		}

	}
}

void ItemChest::OpenChest()
{
	if (m_is_open) {

		if (time <= 1) {
			time += DELTA;

			m_chest_transform[lid].rotation = m_chest_transform[lid].rotation * (1 - time) +
				target_rotation * time;
		}
		else {

			if (!m_is_item_drop) {
				RamdomItemDrop();
				m_is_item_drop = true;
			}
		}
	}
}

void ItemChest::RamdomItemDrop()
{
	
	int index = Utility::Rand(0, 3);
	
	switch (index) {

	case 0:
		new HeartItem(0, Transform(m_Transform.position + CVector3D(0, 1, 0), CVector3D::zero, CVector3D(1, 1, 1)));
		break;
	case 1:
		new AttackSpeedUpItem(1, Transform(m_Transform.position + CVector3D(0, 1, 0), CVector3D::zero, CVector3D(2, 2, 2)));
		break;
	case 2:
		new MoveSpeedUpItem(2, Transform(m_Transform.position + CVector3D(0, 1, 0), CVector3D::zero, CVector3D(0.01,0.01, 0.01)));
		break;
	default:
		break;
	}

}
