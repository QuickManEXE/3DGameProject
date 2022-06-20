#pragma once
#include"DungeonEventBase.h"
#include"../DungeonMarker.h"

class DungeonEventManager {

public:

	//�_���W�����C�x���g�\����
	struct DungeonEvent {
		int parent_room_num;//�C�x���g�����镔���̔ԍ�
		DungeonEventBase* event; //�C�x���g���e
		CAABB aabb;
		DungeonEvent(int _parent_room_num, DungeonEventBase* _event, CAABB _aabb) {
			parent_room_num = _parent_room_num;
			event = _event;
			aabb = _aabb;
		}
	};
	typedef std::vector<DungeonEvent> DungeonEvents;

	DungeonEvents m_dungeon_events;

	//���݃A�N�e�B�u�ȃC�x���g
	DungeonEvent* m_current_event;

	DungeonEventManager();

	void Run();
	
	void CheckCollision(const CVector3D& _pos);

	void SetEvent(DungeonMarker::DungeonData _d_data, float _tile_size);

	void RenderALLAABB();
};