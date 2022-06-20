#pragma once
#include"DungeonEventBase.h"
#include"../DungeonMarker.h"

class DungeonEventManager {

public:

	//ダンジョンイベント構造体
	struct DungeonEvent {
		int parent_room_num;//イベントおこる部屋の番号
		DungeonEventBase* event; //イベント内容
		CAABB aabb;
		DungeonEvent(int _parent_room_num, DungeonEventBase* _event, CAABB _aabb) {
			parent_room_num = _parent_room_num;
			event = _event;
			aabb = _aabb;
		}
	};
	typedef std::vector<DungeonEvent> DungeonEvents;

	DungeonEvents m_dungeon_events;

	//現在アクティブなイベント
	DungeonEvent* m_current_event;

	DungeonEventManager();

	void Run();
	
	void CheckCollision(const CVector3D& _pos);

	void SetEvent(DungeonMarker::DungeonData _d_data, float _tile_size);

	void RenderALLAABB();
};