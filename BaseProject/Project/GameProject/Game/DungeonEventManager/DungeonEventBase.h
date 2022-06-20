#pragma once
#include"../DungeonMarker.h"

class DungeonEventBase {

public:

	DungeonMarker::RoomRect m_room;

	bool is_active;
	bool is_complete;

	DungeonEventBase();
	DungeonEventBase(DungeonMarker::RoomRect _room);

	//イベント開始時処理
	virtual void Enter();

	//イベント中処理
	virtual void Execute();

	//イベント終了時
	virtual void Exit();

};