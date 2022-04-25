#include "NonAnimObject.h"


NonAnimObject::NonAnimObject(UpdatePriority _updatePriority, const char* _modelName, RenderPriority _renderPriority):
Base(_updatePriority, _modelName), mp_ParentModel(nullptr)
{
	m_Model = COPY_RESOURCE(_modelName, CModelObj);

	m_Render.RegistRender(this, (RenderFunc)&NonAnimObject::Render, _renderPriority, _modelName);
}

void NonAnimObject::Update()
{
	if (mp_ParentModel) {

		CMatrix P = mp_ParentModel->GetFrameMatrix(m_ParentIdx);

		m_Transform.position = P.GetPosition();
		CMatrix trans = CMatrix::MTranselate(m_Transform.position);

		m_WorldMatrix = trans * CMatrix::MRotation(CVector3D(DtoR(30), DtoR(180), 0)) * CMatrix::MTranselate(0.21f, -0.1f, -0.1f) * CMatrix::MScale(0.12f, 0.12f, 0.12f);

	}
}

void NonAnimObject::Render()
{
	if (mp_ParentModel) {
		m_Model.Render(m_WorldMatrix);
	}
	else {

		m_Model.SetPos(m_Transform.position);
		m_Model.SetRot(m_Transform.rotation);
		m_Model.SetScale(m_Transform.scale);
		m_Model.Render();

	}

#ifdef _DEBUG
	Utility::DrawSphere(m_Transform.position, 0.1f, CVector4D(1, 1, 1, 0.1f));
	Utility::DebugLocalDir(m_Transform.position, m_Transform.rotation);
#endif // DEBUG
}

void NonAnimObject::SetTransform(const Transform& transform)
{
	this->m_Transform = transform;
}

void NonAnimObject::SetParent(CModel* p, int idx) {

	mp_ParentModel = p;
	m_ParentIdx = idx;

	CMatrix P = mp_ParentModel->GetFrameMatrix(m_ParentIdx);
	//親のスケールに影響され、矢の大きさが変わらないよう
	//親のスケールを打ち消し
	float s = P.GetFront().Length();
	P = P * CMatrix::MScale(s, s, s).GetInverse();
	//親の逆行列からローカル行列を逆算
	//矢が刺さるときに使用
	m_LocalMatrix = P.GetInverse() * m_WorldMatrix;
}

CModelObj& NonAnimObject::GetModel()
{
	return m_Model;
}
