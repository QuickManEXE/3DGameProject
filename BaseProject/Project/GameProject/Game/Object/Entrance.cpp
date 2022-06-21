#include "Entrance.h"
#include"../../StarterAsset.h"

std::vector<Entrance*> Entrance::m_entrances;

Entrance::Entrance(int _entrance_num) : entrance_num(_entrance_num),
	NonAnimObject(UpdatePriority::eUp_Field, StarterAsset::Cylinder, RenderPriority::eRd_Field)
{
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Entrance::CollisionCheck, CollisionPriority::eCol_Field, "Entrance");

	m_rad = 5.0f;

	m_entrances.push_back(this);

	m_door1 = COPY_RESOURCE(StarterAsset::Plate, CModelObj);
	m_door2 = COPY_RESOURCE(StarterAsset::Plate, CModelObj);
}

void Entrance::Update()
{
	this->m_Transform.position;

	m_door1_transform.position = m_Transform.position;

	tank_matrix[eBottom] =
		CMatrix::MTranselate(tank_pos[eBottom]) *
		CMatrix::MRotation(tank_rot[eBottom]);
	tank_matrix[eTop] =
		tank_matrix[eBottom] * CMatrix::MTranselate(0, 3, 0) *
		CMatrix::MRotation(tank_rot[eTop]);
	tank_matrix[eCannon] =
		tank_matrix[eTop] * CMatrix::MTranselate(0, 0.7, 1) *
		CMatrix::MRotation(tank_rot[eCannon]);
}

void Entrance::Render()
{
	m_door1.SetPos(m_Transform.position);
	m_door1.SetRot(m_Transform.rotation);
	m_door1.SetScale(m_Transform.scale);
	m_door1.Render();
}

void Entrance::CollisionCheck(CollisionTask* _task)
{

}
