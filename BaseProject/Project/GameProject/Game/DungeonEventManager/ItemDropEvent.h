#pragma once
#include"DungeonEventBase.h"

class ItemDropEvent : public DungeonEventBase {

public:

	ItemDropEvent(DungeonMarker::RoomRect _room);

	//�C�x���g�J�n������
	virtual void Enter()override;

	//�C�x���g������
	virtual void Execute()override;

	//�C�x���g�I����
	virtual void Exit()override;

};