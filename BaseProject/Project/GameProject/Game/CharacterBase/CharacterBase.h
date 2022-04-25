#pragma once
#include "../AnimObject.h"


//�ӎv�������Ĉړ�����|�[��
class CharacterBase : public AnimObject {
public:
	//�L�����N�^�[���n�ʂɂ��Ă��邩�ǂ���
	bool m_IsGround;
	//�̂̃J�v�Z��
	CCapsule m_Capsule;
public:
	CharacterBase(UpdatePriority _updatePriority,const char* _modelName,RenderPriority _renderPriority);
	virtual void Update();
	
	void AddGravity();

	void AddMoveForce();

	void StandardRender();
	
	void CollisionCharacter(CollisionTask* _task);

	void CollisionField(CollisionTask* _task);
};