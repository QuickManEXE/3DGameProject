#pragma once
#include"../Base/Base.h"

//�A�j���[�V�����̂Ȃ����f�������Q�[���I�u�W�F�N�g
class NonAnimObject : public Base {
protected:

	CModel* mp_ParentModel;
	CMatrix m_LocalMatrix;
	CMatrix m_WorldMatrix;
	int		m_ParentIdx;

	CModelObj m_Model;

	RenderTask m_Render;

public:
	NonAnimObject(UpdatePriority _updatePriority, const char* _modelName, RenderPriority _renderPriority);
	virtual void Update()override;
	virtual void Render();
	void SetTransform(const Transform& transform);
	void SetParent(CModel* p, int idx);
	CModelObj& GetModel();
};