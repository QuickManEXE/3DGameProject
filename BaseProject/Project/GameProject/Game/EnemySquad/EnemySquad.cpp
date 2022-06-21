#include "EnemySquad.h"

EnemySquad::EnemySquad() : Base(UpdatePriority::eUp_Enemy, "EnemySquad") , enemy_num(0),is_active(false)
{
}

EnemySquad::~EnemySquad()
{

}

void EnemySquad::Update()
{
	is_destruction = GetIsDestruction();
}

void EnemySquad::SpawnEnemy(const CVector3D& pos)
{
	is_active = true;

	enemy_num++;

	m_enemys.push_back(new Enemy(Transform(pos, CVector3D::zero, CVector3D(0.01f, 0.01f, 0.01f)), "Golem"));
}

bool EnemySquad::GetIsDestruction()
{
	
	for (int i = 0; i < m_enemys.size(); i++) {

		auto enemy = m_enemys[i];

		if (enemy->m_StateAI.IsCurrentState(EnemyState::DeathState)) {

			m_enemys.erase(m_enemys.begin() + i);

		}

	}
	
	if (m_enemys.size() <= 0 && is_active) {
		return true;
	}else
	return false;
}
