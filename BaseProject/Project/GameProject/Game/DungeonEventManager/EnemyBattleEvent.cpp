#include "EnemyBattleEvent.h"
#include"../Map.h"
#include"../Object/Entrance.h"
#include"../EnemySquad/EnemySquad.h"
#include"../ItemChest/ItemChest.h"

EnemyBattleEvent::EnemyBattleEvent(DungeonMarker::RoomData2 _room) : DungeonEventBase(_room)
{
}

void EnemyBattleEvent::Enter()
{
	//敵の出現
	printf("敵とのバトルイベント開始\n");

	const DungeonMarker::RoomData2 room2 = m_room2;

	int num = DungeonMarker::GetRectGridNum(room2.room_rect);

	std::vector<CVector3D>positions = DungeonMarker::GetRoomPosition(room2.room_rect);

	int enemy_num = num / 10;	

	m_squad = new EnemySquad();

	for (int i = 0; i < enemy_num; i++) {
		int index = Utility::Rand(0, (int)positions.size());
		CVector3D pos = positions[index];
		
		m_squad->SpawnEnemy(pos * TILE_SIZE);

		//m_enemys.push_back(new Enemy(Transform((pos * TILE_SIZE) + CVector3D(0, 1, 0), CVector3D::zero, CVector3D(0.01f, 0.01f, 0.01f)), "Golem"));
		positions.erase(positions.begin() + index);
	}


	for (auto ent : Entrance::m_entrances) {

		ent->SetIsBlock(true);

	}

	
}

void EnemyBattleEvent::Execute()
{
	if (m_squad->is_destruction) {
		is_complete = true;
	}
}

void EnemyBattleEvent::Exit()
{
	for (auto ent : Entrance::m_entrances) {

		ent->SetIsBlock(false);

	}

	auto m_room = m_room2.room_rect;

	int width = (m_room.m_right - m_room.m_left) / 2;
	int height = (m_room.m_bottom - m_room.m_top) / 2;
	CVector3D pos(m_room.m_left + width, -0.5, m_room.m_top + height);
	new ItemChest(Transform(pos * TILE_SIZE, CVector3D::zero, CVector3D(1, 1, 1)));

	is_active = false;
}
