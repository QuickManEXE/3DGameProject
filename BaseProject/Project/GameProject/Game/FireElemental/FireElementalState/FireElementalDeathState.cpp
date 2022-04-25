#include "FireElementalDeathState.h"
#include"../../GameManager/GameManager.h"
#include"../../Player/Player.h"
#include"../../../Effect/Effect3D.h"

FireElementalDeathState::CameraPosData FireElementalDeathState::m_posData[FireElementalDeathState::data_max]{
	{CVector3D(30,10,0),CVector3D(0,10,0)},
	{CVector3D(-30,10,0),CVector3D(0,10,0)},
	{CVector3D(0,10,30),CVector3D(0,10,0)},
};


FireElementalDeathState::FireElementalDeathState(FireElemental* owner) : State(owner)
{
}

void FireElementalDeathState::Enter()
{
	t = 0;

	
	Camera::GetInstance()->SetCamera(this);

	owner->m_model.ChangeAnimation(6,false);
	
	owner->m_Transform.position = CVector3D(0, 0, 0);
	owner->m_Transform.rotation = CVector3D(0, DtoR(0), 0);

	if (Player* p = Player::GetInstance()) {
		p->m_Transform.position = CVector3D(0, 0, 40);

		p->m_StateAI.ChangeState(PlayerState::StayState);
	}

	is_effect = false;

	
}

void FireElementalDeathState::Execute()
{
	t += DELTA;

	
	if (t >= 1.0f) {

		if (m_idx<2) {
			m_idx++;
			

		}
		else {
			
			Camera::GetInstance()->SetCamera(nullptr);
			GameManager::GetInstance()->m_StateAI.ChangeState(GameManager::GameManagerState::GameClearState);


		}
		
		t = 0.0f;
	} 
	


}

void FireElementalDeathState::Exit()
{
}

void FireElementalDeathState::Death()
{
}

void FireElementalDeathState::UpdateCamera()
{
	CCamera::GetCurrent()->LookAt(m_posData[m_idx].pos, m_posData[m_idx].at, CVector3D(0, 1, 0));
}
