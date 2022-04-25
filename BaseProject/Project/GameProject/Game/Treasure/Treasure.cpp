#include "Treasure.h"

Treasure::Treasure(const Transform& transform) 
	: Base(UpdatePriority::eUp_Field, "GameObject")
{
	this->m_Transform = transform;

	m_model = COPY_RESOURCE("Treasure", CModelObj);

	// •`‰æƒ^ƒXƒN‚É“o˜^
	m_render.RegistRender(this, (RenderFunc)&Treasure::Render, RenderPriority::eRd_Field, "Treasure");
}

void Treasure::Render()
{
	m_model.SetPos(m_Transform.position);
	m_model.SetRot(m_Transform.rotation);
	m_model.SetScale(m_Transform.scale);
	m_model.Render();
}
