#pragma once
#include"../DungeonMarker.h"

class DungeonEventBase {

public:

	DungeonMarker::RoomRect m_room;

	bool is_active;
	bool is_complete;

	DungeonEventBase();
	DungeonEventBase(DungeonMarker::RoomRect _room);

	//�C�x���g�J�n������
	virtual void Enter();

	//�C�x���g������
	virtual void Execute();

	//�C�x���g�I����
	virtual void Exit();

};