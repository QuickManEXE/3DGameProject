#include"Goal.h"
#include"../../StarterAsset.h"
#include"../CharacterBase/CharacterBase.h"
#include"../GameManager/GameManager.h"

Goal::Goal():
	StaticMeshObject(UpdatePriority::eUp_Field,"Goal", StarterAsset::Cone, RenderPriority::eRd_Field)
{
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Goal::CollisionCheck, CollisionPriority::eCol_Field,"Goal");

	m_rad = 5.0f;
}

void Goal::CollisionCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		CVector3D dir;
		float dist;

		if (CCollision::CollisionCapsuleShpere(c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad,m_Transform.position,m_rad)) {

			if (CInput::GetState(0, CInput::ePush, CInput::eButton1)){
				printf("ÉSÅ[Éã");
				GameManager::Instance().m_StateAI.ChangeState(GameManager::GameManagerState::BeginGameState);	
			}
		}
	}

}
