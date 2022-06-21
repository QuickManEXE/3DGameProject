#pragma once
#include"../DungeonMarker.h"

class DungeonEventBase {

public:

	DungeonMarker::RoomData2 m_room2;

	bool is_active;
	bool is_complete;

	DungeonEventBase();
	DungeonEventBase(DungeonMarker::RoomData2 _room2);

	//�C�x���g�J�n������
	virtual void Enter();

	//�C�x���g������
	virtual void Execute();

	//�C�x���g�I����
	virtual void Exit();

};