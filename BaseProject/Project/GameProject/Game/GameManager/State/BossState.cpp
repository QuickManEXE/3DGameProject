#include "BossState.h"
#include"../../Player/Player.h"
#include"../../GUI.h"
#include"../../Geometry.h"
#include"../../../SoundResource.h"
#include"../../SkyBox.h"

BossState::BossState(GameManager* owner) : State(owner)
{
}

void BossState::Enter()
{
	TaskManager::GetInstance()->AllKill();
	
	new SkyBox();

	if (Player* p = dynamic_cast<Player*> (TaskManager::GetInstance()->FindTask(UpdatePriority::eUp_Player))) {
		p->m_Transform.position = CVector3D(0, 0, 40);
	}

	mp_field = new Geometry(Transform(CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(50, 50, 50)), "Cave");

	mp_fire = new FireElemental(Transform(CVector3D(0, 0, 10), CVector3D(0, DtoR(0), 0), CVector3D::one), "FireElemental");
	
	SOUND(SoundResource::BGM_LostPlace.c_str())->Stop();
	SOUND(SoundResource::BGM_BossBattle.c_str())->Play(true);
}

void BossState::Execute()
{
	if (!mp_field) {
		printf("‚«‚¦‚Ü‚µ‚½\n");
	}
}

void BossState::Exit()
{
	
}

void BossState::Render()
{
}

void BossState::CollisionCheck(CollisionTask* task)
{
}
