#include"Goal.h"
#include"../../StarterAsset.h"
#include"../CharacterBase/CharacterBase.h"
#include"../GameManager/GameManager.h"
#include"../../Effect/EffectCollection.h"
#include"../Player/Player.h"

Goal::Goal():
	StaticMeshObject(UpdatePriority::eUp_Field,"Goal", "Portal", RenderPriority::eRd_Field)
{
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Goal::CollisionCheck, CollisionPriority::eCol_Field,"Goal");

	m_rad = 5.0f;

	m_time = 0;

	m_is_teleport = false;
}

void Goal::CollisionCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		CVector3D dir;
		float dist;

		if (CCollision::CollisionCapsuleShpere(c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad,m_Transform.position,m_rad)) {

			if (CInput::GetState(0, CInput::ePush, CInput::eButton1)&&!m_is_teleport){
				printf("ƒS[ƒ‹");
				EffectCollection::BulletSplash3D(c->m_Transform.position + CVector3D(0,1,0), nullptr);
				Player::GetInstance()->m_StateAI.ChangeState(PlayerState::StayState);
				SetIsTeleport(true);
			}
		}
	}

}

void Goal::Update()
{
	Teleport();
}

void Goal::Teleport()
{
	if (m_is_teleport) {

		if (m_time <= 5.0f) {
			m_time += DELTA;
		}
		else {

			SetTime(0);
			SetIsTeleport(false);
			GameManager::Instance().m_StateAI.ChangeState(GameManager::GameManagerState::ExecuteState);
		}

	}
}
