#pragma once
#include"../Base/Base.h"
#include"../Singleton/Singleton.h"
#include"DungeonMarker.h"

#define MAP_WIDTH  25
#define MAP_HEIGHT 25
#define TILE_SIZE 6.0f

class Map : public Base,public Singleton<Map>{
public:
	friend class Singleton<Map>;
protected:
	DungeonMarker::DungeonData m_dungeon_data;

	RenderTask m_render;

	CollisionTask m_collide;

	CModelObj m_wall;

	CModelObj m_ground;
	
	Map();
public:
	void Render();
	void CollisionCheck(CollisionTask* _task);
	DungeonMarker::DungeonData GetDungeonData() {
		return m_dungeon_data;
	}
	void CreateDungeon(const CVector2D& _dungeon_size, const CVector2D& _first_room_size, const CVector2D& _first_room_pos, int _max_room_num);
	DungeonMarker::TileType Map::GetTip(const CVector3D& pos, int* ix, int* iy);
	
};