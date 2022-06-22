#include "ItemDropEvent.h"
#include"../Geometry.h"
#include"../Map.h"
#include"../Item/ItemBase.h"
#include"../ItemChest/ItemChest.h"

ItemDropEvent::ItemDropEvent(DungeonMarker::RoomData2 _room) : DungeonEventBase(_room)
{
}

void ItemDropEvent::Enter()
{
	auto m_room = m_room2.room_rect;
	
	int width = (m_room.m_right - m_room.m_left) /2;
	int height = (m_room.m_bottom -m_room.m_top) /2;
	CVector3D pos(m_room.m_left + width, -0.5, m_room.m_top + height);
	//new ItemBase(0,Transform(pos * TILE_SIZE, CVector3D::zero, CVector3D(1, 1, 1)), StarterAsset::Sphere);
	new ItemChest(Transform(pos * TILE_SIZE, CVector3D::zero, CVector3D(1, 1, 1)));
	is_complete = true;
	is_active = false;
}

void ItemDropEvent::Execute()
{


}

void ItemDropEvent::Exit()
{


}
