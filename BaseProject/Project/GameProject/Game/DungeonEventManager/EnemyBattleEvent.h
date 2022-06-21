#pragma once
#include"DungeonEventBase.h"
#include"../Enemy/Enemy.h"
#include"../EnemySquad/EnemySquad.h"

class EnemyBattleEvent : public DungeonEventBase {
private:

	EnemySquad* m_squad;

public:

	EnemyBattleEvent(DungeonMarker::RoomData2 _room);

	//イベント開始時処理
	virtual void Enter()override;

	//イベント中処理
	virtual void Execute()override;

	//イベント終了時
	virtual void Exit()override;

};