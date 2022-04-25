#include "Wire.h"


Wire::Wire(const Transform& transform, float length, Wire* wire) : Base(eUp_Collision,"Rope")
{
	m_model = COPY_RESOURCE("Rope", CModelObj);

	m_render.RegistRender(this, (RenderFunc)&Wire::Render, RenderPriority::eRd_Collision, "NewRope");

	
	m_rad = 0.1f;

	this->m_parent_wire = wire;

	this->m_Transform = transform;

	if (m_parent_wire) {

		this->lineS = m_parent_wire->lineE;
		this->m_Transform.rotation = m_parent_wire->m_Transform.rotation * 0.9f;
	}
	else {


		this->lineS = transform.position;

	}

	this->rope_length = length;

	m_top_matrix = CMatrix::MRotation(transform.rotation);

	lineE = lineS + -m_top_matrix.GetUp() * rope_length;
}

void Wire::Update()
{
	CMatrix m_top_matrix = CMatrix::MRotation(m_Transform.rotation);

	if (m_parent_wire) {

		this->lineS = m_parent_wire->lineE;

	}
	else {


		this->lineS = m_Transform.position;

	}

	lineE = lineS + -m_top_matrix.GetUp() * rope_length;
}

void Wire::Render()
{
	//Utility::DrawCapsule(lineS,lineE,m_rad,CVector4D(0, 0, 0.5, 0.5f));

	m_model.SetPos(lineS);
	m_model.SetRot(m_Transform.rotation + CVector3D(DtoR(0), 0, DtoR(0)));
	m_model.SetScale(CVector3D(0.2f, 0.2f, 0.2f));
	m_model.Render();
}
