#include "Entrance.h"
#include"../../StarterAsset.h"
#include"../CharacterBase/CharacterBase.h"
#include"../../Effect/EffectCollection.h"

std::vector<Entrance*> Entrance::m_entrances;

Entrance::Entrance(int _entrance_num,const Transform& _transform) : entrance_num(_entrance_num),
	StaticMeshObject(UpdatePriority::eUp_Field,"Entrance", StarterAsset::Cube,RenderPriority::eRd_Field)
	,m_is_open(false),m_is_block(false)
{
	m_Transform = _transform;

	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Entrance::CollisionCheck, CollisionPriority::eCol_Field, "Entrance");

	m_rad = 5.0f;

	m_entrances.push_back(this);

	m_door = COPY_RESOURCE(StarterAsset::Cube , CModelObj);
	
	m_door_transform[left_door].position = CVector3D(0, 0, 0);
	m_door_transform[right_door].position = CVector3D(1.5, 0, 0);

	m_door_transform[left_door].scale = CVector3D(3, 3, 3);

	target_move = CVector3D(0, 3, 0);

	target_pos = m_Transform.position;

	origin_pos = m_Transform.position;

	time = 0;

	m_rad = 2;

	m_block_rad = 4.0f;

	effect_time = 1.0;
}

void Entrance::Update()
{
	OpenEntrance();
	BlockDust();
}

void Entrance::Render()
{
	m_Model.SetPos(m_Transform.position);
	m_Model.SetRot(m_Transform.rotation);
	m_Model.SetScale(m_Transform.scale);
	m_Model.Render();


	if (m_is_block) {

		//Utility::DrawSphere(origin_pos, m_block_rad, CColorRGBA(0.5f, 0.5f, 0, 0.5f));

	}
	
}

void Entrance::CollisionCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		
		float dist;

		if (CCollision::CollisionCapsuleShpere(c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, m_Transform.position, m_rad)) {

			if (CInput::GetState(0, CInput::ePush, CInput::eButton1) && !m_is_open) {
				
				m_is_open = true;
				target_pos = m_Transform.position + target_move;
			}
		}

		if (m_is_block) {
			CVector3D push_back(0,0,0);
			//••½ó‘Ô‚È‚ç
			if (Utility::CollisionCapsuleSphere(c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, origin_pos, m_block_rad,&push_back)) {

				c->m_Transform.position -= push_back;
				
			}
		}
	}

}

void Entrance::OpenEntrance()
{
	if (m_is_open) {

		if (time <= 1) {
			time += DELTA;

			m_Transform.position = m_Transform.position * (1 - time) +
				target_pos * time;
		}
	}
	else {

		if (time > 0) {
			time -= DELTA;

			m_Transform.position = m_Transform.position * (1 - time) +
				origin_pos * time;
		}


	}
}

void Entrance::BlockDust()
{

	if (m_is_block) {
		
		effect_time -= DELTA;

		if (effect_time <= 0) {

			EffectCollection::DustSplash3D(origin_pos);
			EffectCollection::DustSplash3D(origin_pos);
			EffectCollection::DustSplash3D(origin_pos);
			effect_time = 1.0f;
		}
	}
}
