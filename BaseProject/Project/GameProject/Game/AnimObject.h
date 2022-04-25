#pragma once
#include"../Base/Base.h"

//�A�j���[�V�����̂��郂�f���������I�u�W�F�N�g
class AnimObject : public Base {
protected:
	
	CModelA3M m_Model;

	RenderTask m_Render;

public:

	AnimObject(UpdatePriority _updatePriority,const char* _modelName,RenderPriority _renderPriority);

	virtual void Update()override;

	virtual void Render();

	void SetTransform(const Transform& _transform);

	void AnimChange(int _num, bool _isLoop);

	CModelA3M* GetModel();
};