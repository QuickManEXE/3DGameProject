#pragma once
#include"DungeonEventBase.h"

class ItemDropEvent : public DungeonEventBase {

public:

	ItemDropEvent(DungeonMarker::RoomData2 _room);

	//イベント開始時処理
	virtual void Enter()override;

	//イベント中処理
	virtual void Execute()override;

	//イベント終了時
	virtual void Exit()override;

};