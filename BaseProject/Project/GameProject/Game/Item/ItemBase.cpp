#include "ItemBase.h"
#include"../CharacterBase/CharacterBase.h"


ItemBase::ItemBase(int _item_id, const Transform& transform, const char* model_name) :
	StaticMeshObject(UpdatePriority::eUp_Field, "Item", model_name, RenderPriority::eRd_Field),m_item_id(_item_id)
{
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&ItemBase::CollisionCheck, CollisionPriority::eCol_Field, model_name);

	SetTransform(transform);
	m_Model.SetPos(transform.position);
	m_Model.SetRot(transform.rotation);
	m_Model.SetScale(transform.scale);
	m_Model.UpdateMatrix();

	m_rad = 1.0f;


}

void ItemBase::CollisionCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		if (CCollision::CollisionCapsuleShpere(c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, m_Transform.position, m_rad)) {


			printf("ƒAƒCƒeƒ€‚Æ‚è‚Ü‚µ‚½");
			SetKill();
		}
	}
}
