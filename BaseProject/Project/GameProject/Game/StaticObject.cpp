#include"StaticObject.h"
#include"CharacterBase/CharacterBase.h"

StaticObject::StaticObject(const Transform& transform, const char* model_name,float rad,float height) 
	: Base(UpdatePriority::eUp_Collision,"Base")
{
	this->m_Transform = transform;

	// •`‰æƒ^ƒXƒN‚É“o˜^
	m_render.RegistRender(this, (RenderFunc)&StaticObject::Render, RenderPriority::eRd_Collision, "StaticObject");
	// “–‚½‚è”»’èƒ^ƒXƒN‚É“o˜^
	m_collision.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&StaticObject::CollisionCheck, CollisionPriority::eCol_Collision, "StaticObject");

	m_model = COPY_RESOURCE(model_name, CModelObj);
	m_model.SetPos(transform.position);
	m_model.SetRot(transform.rotation);
	m_model.SetScale(transform.scale);
	m_model.UpdateMatrix();

	m_sphere.Set(transform.position + CVector3D(0, rad, 0), rad);

	m_capsule.Set(transform.position+CVector3D(0,rad,0), transform.position + CVector3D(0, height, 0));
	m_capsule.SetRad(rad);
}

void StaticObject::Update()
{

	m_sphere.Set(m_Transform.position + CVector3D(0, m_sphere.m_rad, 0), m_sphere.m_rad);

	
}

void StaticObject::Render()
{
	m_model.SetPos(m_Transform.position);
	m_model.SetRot(m_Transform.rotation);
	m_model.SetScale(m_Transform.scale);
	m_model.Render();

	DebugInfo();
}

void StaticObject::DebugInfo()
{
#ifdef _DEBUG

	//Utility::DrawSphere(m_sphere.m_center, m_sphere.m_rad, CColorRGBA(1, 1, 0, 0.5f));

	Utility::DrawCapsule(m_capsule.m_start, m_capsule.m_end, m_capsule.m_rad,CColorRGBA(1, 1, 0, 0.1f));
#endif // _DEBUG
}

void StaticObject::CollisionCheck(CollisionTask* task)
{
	if (task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (task->GetTask())) {

		CVector3D dir;
		float dist;

		if (CCollision::CollisionCapsule(m_capsule.m_start, m_capsule.m_end, m_capsule.m_rad,
			c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, &dist, &dir)) {

			float s = m_capsule.m_rad + c->m_Capsule.m_rad - dist;

			c->m_Transform.position += dir * s;

		}

	}
	

}
