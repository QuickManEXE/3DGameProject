#include "AnimObject.h"

AnimObject::AnimObject(UpdatePriority _updatePriority,const char* _modelName, RenderPriority _renderPriority) :
	Base(_updatePriority, _modelName)
{
	m_Model = COPY_RESOURCE(_modelName, CModelA3M);

	m_Render.RegistRender(this, (RenderFunc)&AnimObject::Render, _renderPriority, _modelName);
}

void AnimObject::Update()
{
	m_Model.UpdateAnimation();
}

void AnimObject::Render()
{
	//キャラクター表示
	m_Model.SetPos(m_Transform.position);
	m_Model.SetRot(m_Transform.rotation);
	m_Model.SetScale(m_Transform.scale);
	m_Model.Render();
}

void AnimObject::SetTransform(const Transform& _transform)
{
	this->m_Transform = _transform;
}

void AnimObject::AnimChange(int _num,bool _isLoop)
{
	m_Model.ChangeAnimation(_num, _isLoop);
}

CModelA3M* AnimObject::GetModel(){
	return &m_Model;
}
