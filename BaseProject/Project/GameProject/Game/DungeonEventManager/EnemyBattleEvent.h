#pragma once
#include"DungeonEventBase.h"
#include"../Enemy/Enemy.h"
#include"../EnemySquad/EnemySquad.h"

class EnemyBattleEvent : public DungeonEventBase {
private:

	EnemySquad* m_squad;

public:

	EnemyBattleEvent(DungeonMarker::RoomData2 _room);

	//�C�x���g�J�n������
	virtual void Enter()override;

	//�C�x���g������
	virtual void Execute()override;

	//�C�x���g�I����
	virtual void Exit()override;

};