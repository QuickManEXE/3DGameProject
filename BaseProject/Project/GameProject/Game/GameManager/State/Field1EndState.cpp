#include "Field1EndState.h"
#include"../../Player/Player.h"
#include"../../../SoundResource.h"
#include"../../../Effect/EffectCollection.h"

Field1EndState::Field1EndState(GameManager* owner) : State(owner)
{
	m_event = CSphere(CVector3D(-47, 20, 8), 3.0f);
}

void Field1EndState::Enter()
{

	Player::GetInstance()->m_StateAI.ChangeState(PlayerState::StayState);

	t = 10.0f;

	event_flag = false;

	Camera::GetInstance()->SetCamera(this);

	SOUND(SoundResource::SE_EarthQuake.c_str())->Play();

	
}

void Field1EndState::Execute()
{

	if (!event_flag) {
		
		t -= DELTA;

		BreakRock();

		if((int)t%2==0)EffectCollection::SmokeWave3D(owner->m_Rock->m_Transform.position+CVector3D(0,5.0f,0));

		if (t <= DELTA) {

			event_flag = true;

			Camera::GetInstance()->SetCamera(Player::GetInstance());
			Player::GetInstance()->m_StateAI.ChangeState(PlayerState::IdleState);
			owner->m_Rock->SetKill();
		}



	}


}

void Field1EndState::Exit()
{
}

void Field1EndState::Field1Update()
{
}

void Field1EndState::Render()
{
#ifdef _DEBUG

	Utility::DrawSphere(m_event.m_center, m_event.m_rad, CColorRGBA(1.0f, 0, 0, 1.0f));

#endif // _DEBUG
}

void Field1EndState::CollisionCheck(CollisionTask* task)
{
	if (task->GetPriority() != eCol_Player)return;

	if (Player* p = dynamic_cast<Player*> (task->GetTask())) {

		if (CCollision::CollisionCapsuleShpere(p->m_Capsule.m_start, p->m_Capsule.m_end, p->m_Capsule.m_rad,
			m_event.m_center, m_event.m_rad))
		{

			owner->m_StateAI.ChangeState(GameManager::GameManagerState::BossBattle);

		}

	}
}

void Field1EndState::BreakRock()
{
	owner->m_Rock->m_Transform.Translate(CVector3D(0, -DELTA, 0));
}

void Field1EndState::UpdateCamera()
{


	CCamera::GetCurrent()->LookAt(CVector3D(-47, 28, 40), CVector3D(-47, 26, 15), CVector3D(0.0f, 1.0f, 0.0f));


}
