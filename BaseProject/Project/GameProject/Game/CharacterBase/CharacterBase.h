#pragma once
#include "../SkeletalMeshObject.h"


//意思をもって移動するポーン
class CharacterBase : public SkeletalMeshObject {
public:
	//キャラクターが地面についているかどうか
	bool m_IsGround;
	//体のカプセル
	CCapsule m_Capsule;
public:
	CharacterBase(UpdatePriority _updatePriority, const char* _name, const char* _modelName,RenderPriority _renderPriority);
	virtual void Update();
	
	void AddGravity();

	void AddMoveForce();

	void StandardRender();
	
	void CollisionCharacter(CollisionTask* _task);

	void CollisionField(CollisionTask* _task);
};