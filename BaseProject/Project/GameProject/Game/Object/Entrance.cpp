#include "Entrance.h"
#include"../../StarterAsset.h"

std::vector<Entrance*> Entrance::m_entrances;

Entrance::Entrance(int _entrance_num) : entrance_num(_entrance_num),
	NonAnimObject(UpdatePriority::eUp_Field, StarterAsset::Cylinder, RenderPriority::eRd_Field)
{
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Entrance::CollisionCheck, CollisionPriority::eCol_Field, "Entrance");

	m_rad = 5.0f;

	m_entrances.push_back(this);
}

void Entrance::CollisionCheck(CollisionTask* _task)
{

}
