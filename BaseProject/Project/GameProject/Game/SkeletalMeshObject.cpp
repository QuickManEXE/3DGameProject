#include "SkeletalMeshObject.h"

SkeletalMeshObject::SkeletalMeshObject(UpdatePriority _updatePriority, const char* _name,const char* _modelName, RenderPriority _renderPriority) :
	Base(_updatePriority, _name)
{
	m_Model = COPY_RESOURCE(_modelName, CModelA3M);

	m_Render.RegistRender(this, (RenderFunc)&SkeletalMeshObject::Render, _renderPriority, _modelName);
}

void SkeletalMeshObject::Update()
{
	m_Model.UpdateAnimation();
}

void SkeletalMeshObject::Render()
{
	//キャラクター表示
	m_Model.SetPos(m_Transform.position);
	m_Model.SetRot(m_Transform.rotation);
	m_Model.SetScale(m_Transform.scale);
	m_Model.Render();
}

void SkeletalMeshObject::SetTransform(const Transform& _transform)
{
	this->m_Transform = _transform;
}

void SkeletalMeshObject::AnimChange(int _num,bool _isLoop)
{
	m_Model.ChangeAnimation(_num, _isLoop);
}

CModelA3M* SkeletalMeshObject::GetModel(){
	return &m_Model;
}
