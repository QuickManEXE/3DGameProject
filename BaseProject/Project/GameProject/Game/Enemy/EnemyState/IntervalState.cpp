#include "IntervalState.h"
#include"../../Player/Player.h"

IntervalState::IntervalState(Enemy* owner) : State(owner)
{
}

void IntervalState::Enter()
{
	
}

void IntervalState::Execute()
{
	if (m_route.empty()) {

		m_route = owner->m_Dijkstra.FindShortestPath(owner->m_Transform.position, owner->m_PopPos);
		auto itr = m_route.end();
		itr--;
		m_next_pos = (*itr)->m_pos;

	}
	else {

		CVector3D dir = m_next_pos - owner->m_Transform.position;

		if (dir.Length() >= 1.0f) {

			float speed = 30.0f;

			float ang = atan2(dir.x, dir.z);

			owner->m_Transform.rotation = (CVector3D(0, ang, 0));

			owner->m_Transform.m_pos_vec += dir.GetNormalize() * DELTA * speed;

			owner->GetModel()->ChangeAnimation(18);
		}
		else {
			
			//‹——£‚ª’Z‚­‚È‚Á‚½‚ç

			//ŽŸ‚Ìƒm[ƒh‚ÉØ‚è‘Ö‚¦‚é
			//Å‰‚Ìƒm[ƒh‚ðÁ‚·
			m_route.pop_back();

			if (!m_route.empty()) {

				auto itr = m_route.end();
				itr--;
				m_next_pos = (*itr)->m_pos;

			}
			else {
				//‚à‚µI“_‚È‚ç
				owner->m_StateAI.ChangeState(EnemyState::SearchState);
			}

			
		}
	}
	

	owner->AddMoveForce();
	owner->AddGravity();
	owner->GetModel()->UpdateAnimation();
}

void IntervalState::Exit()
{
}

