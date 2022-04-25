#pragma once
#include"../Base/Base.h"
#include"DungeonMarker.h"

#define TILE_SIZE 4.0f

class Map : public Base{
private:
	DungeonMarker::DungeonData m_dungeon_data;

	RenderTask m_render;

	CollisionTask m_collide;

	CModelObj m_wall;

	CModelObj m_ground;
public:
	Map();
	void Render();
	void CollisionCheck(CollisionTask* _task);
};