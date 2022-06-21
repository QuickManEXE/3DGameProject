#include "DungeonEventManager.h"
#include"ItemDropEvent.h"
#include"EnemyBattleEvent.h"

DungeonEventManager::DungeonEventManager() :  m_current_event(nullptr)
{

}

void DungeonEventManager::Run()
{
	if (m_current_event) {

		if (m_current_event->event->is_complete) {
			m_current_event->event->Exit();
			m_current_event = nullptr;
		}
		else {
			m_current_event->event->Execute();

		}
		
	}

}

void DungeonEventManager::CheckCollision(const CVector3D& _pos)
{
	for (DungeonEvent& event : m_dungeon_events) {

		if (event.event->is_active || event.event->is_complete)continue;

		if (CCollision::CollisionAABBPoint(event.aabb, _pos)) {

			int num = event.parent_room_num;
			printf("\n%dの部屋イベント開始\n",num);

			event.event->is_active = true;

			m_current_event = &event;
			
			event.event->Enter();

		}
	}


}

void DungeonEventManager::SetEvent(DungeonMarker::DungeonData _d_data,float _tile_size)
{
	DungeonMarker::DungeonData d_data = _d_data;
	DungeonMarker::RoomData rooms = d_data.m_room;
	DungeonMarker::RoomDatas2 rooms2 = d_data.m_rooms2;
	float tile_size = _tile_size;
	
	for (int i = 0; i < rooms2.size() ; i++) {

		const DungeonMarker::RoomRect room = rooms2[i].room_rect;
		int left = room.m_left;
		int right = room.m_right-1;
		int top = room.m_top;
		int bottom = room.m_bottom-1;

		int grid_num = DungeonMarker::GetRectGridNum(room);

		if(grid_num<=8){
			
			m_dungeon_events.push_back(DungeonEvent(i, new ItemDropEvent(rooms2[i]),
				CAABB(CVector3D(left, -tile_size, top) * tile_size - CVector3D(tile_size / 2.0f, 0, tile_size / 2.0f) + CVector3D(0.5f, 0, 0.5f),
					CVector3D(right, tile_size, bottom) * tile_size + CVector3D(tile_size / 2.0f, 0, tile_size / 2.0f) - CVector3D(0.5f, 0, 0.5f))));

		
		}
		else if (grid_num <= 100) {

			m_dungeon_events.push_back(DungeonEvent(i, new EnemyBattleEvent(rooms2[i]),
				CAABB(CVector3D(left, -tile_size, top) * tile_size - CVector3D(tile_size / 2.0f, 0, tile_size / 2.0f) + CVector3D(0.5f, 0, 0.5f),
					CVector3D(right, tile_size, bottom) * tile_size + CVector3D(tile_size / 2.0f, 0, tile_size / 2.0f) - CVector3D(0.5f, 0, 0.5f))));


		}

		

	}

}

void DungeonEventManager::RenderALLAABB()
{
	for (const DungeonEvent& event : m_dungeon_events) {

		CAABB aabb = event.aabb;
		aabb.Draw(CColorRGBA(1,1,0,0.5f));

	}
	

}

void DungeonEventManager::ChoiceEvent(int grid_num)
{
}
