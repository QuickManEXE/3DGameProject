#include"GameTestState.h"
#include"../../Player/Player.h"
#include"../../Geometry.h"
#include"../../SkyBox.h"
#include"../../Map.h"
#include"../../GUI.h"
#include"../../FireElemental/FireElemental.h"
#include"../../Object/Goal.h"

GameTestState::GameTestState(GameManager* owner) : State(owner)
{
}

void GameTestState::Enter()
{
	owner->m_CurrentDungeonNum = 1;
	owner->m_ClearDungeonNum = 5;

	new SkyBox();

	new Player(Transform(CVector3D(0, 0, 0), CVector3D(0, DtoR(0), 0), CVector3D(0.01f, 0.01f, 0.01f)), "Archer2");

	Player::GetInstance()->m_StateAI.ChangeState(PlayerState::IdleState);

	new Geometry(Transform(CVector3D::zero, CVector3D::zero, CVector3D(10,10,10)), StarterAsset::Plate);

	Map::Build();

	Map::Instance().CreateDungeon(CVector2D(MAP_WIDTH,MAP_HEIGHT),CVector2D(4,4),CVector2D(MAP_WIDTH/2,MAP_HEIGHT/2),10);

	CVector3D player_pos;

	if (DungeonMarker::GetRandomDungeonPos(&Map::Instance().GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		CVector3D pos(player_pos.x * TILE_SIZE, 0, player_pos.z * TILE_SIZE);
		
		Player::GetInstance()->m_Transform.position = pos;
	};


	GUI::Build();

	GUI::Instance().InitMiniMap(Map::Instance().GetDungeonData());

	Goal::Build();

	if (DungeonMarker::GetRandomDungeonPos(&Map::Instance().GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		
		Goal::Instance().m_Transform = Transform(player_pos * TILE_SIZE + CVector3D(0,-1,0), CVector3D::zero, CVector3D::one);

	};

	
	


}

void GameTestState::Execute()
{
}

void GameTestState::Exit()
{
}
