#include"Field1State.h"
#include"../../Player/Player.h"
#include"../../Enemy/Enemy.h"
#include"../../GUI.h"
#include"../../../SoundResource.h"
#include"../../Geometry.h"
#include"../../Rope/Rope.h"

Field1State::Field1State(GameManager* owner) : State(owner)
{
	m_event = CSphere(CVector3D(-47, 20, 8), 3.0f);
}

void Field1State::Enter()
{
	new GUI();

	Player::GetInstance()->m_StateAI.ChangeState(PlayerState::IdleState);

	{//“G‚ÌoŒ»

		new Enemy(Transform(CVector3D(0, 10, 40), CVector3D::zero, CVector3D(0.01f, 0.01f, 0.01f)), "Golem");
		new Enemy(Transform(CVector3D(15, 10, 50), CVector3D::zero, CVector3D(0.01f, 0.01f, 0.01f)), "Golem");
		new Enemy(Transform(CVector3D(-30, 10, 55), CVector3D::zero, CVector3D(0.01f, 0.01f, 0.01f)), "Golem");
		new Enemy(Transform(CVector3D(-45, 10, 35), CVector3D::zero, CVector3D(0.01f, 0.01f, 0.01f)), "Golem");
		new Enemy(Transform(CVector3D(-55, 20, 48), CVector3D::zero, CVector3D(0.01f, 0.01f, 0.01f)), "Golem");

	}

	auto enemys = TaskManager::GetInstance()->FindTasks("Enemy");

	owner->m_EnemyCount = enemys.size();

	t = 5.0f;
}

void Field1State::Execute()
{

	if (owner->m_EnemyCount <= 0) {

		
		t -= DELTA;

		if(t<=0)owner->m_StateAI.ChangeState(GameManager::GameManagerState::Field1EndState);

	}

	if (Player::GetInstance()->m_Transform.position.y <= -100) {

		GameManager::Instance().m_StateAI.ChangeState(GameManager::GameManagerState::GameOverState);


	}

}

void Field1State::Exit()
{

}

