#pragma once
#include "../stdafx.h"
#include"../Global.h"
#include"../GID.h"
#include"../TaskSystem/TaskInclude.h"
#include"../GameComponent/Transform.h"
#include"../Resource.h"


class Base : public Task {
public:
	Transform m_Transform;
public:
	//�R���X�g���N�^
	Base(int _priority, const char* _name);
	//�f�X�g���N�^
	virtual~Base();
};