#pragma once
#include"../DungeonMarker.h"

class DungeonEventBase {

public:

	DungeonMarker::RoomData2 m_room2;

	bool is_active;
	bool is_complete;

	DungeonEventBase();
	DungeonEventBase(DungeonMarker::RoomData2 _room2);

	//イベント開始時処理
	virtual void Enter();

	//イベント中処理
	virtual void Execute();

	//イベント終了時
	virtual void Exit();

};