#include"GameTestState.h"
#include"../../Player/Player.h"
#include"../../Geometry.h"
#include"../../SkyBox.h"
#include"../../Map.h"
#include"../../GUI.h"
#include"../../FireElemental/FireElemental.h"

GameTestState::GameTestState(GameManager* owner) : State(owner)
{
}

void GameTestState::Enter()
{
	new SkyBox();

	new Player(Transform(CVector3D(0, 0, 0), CVector3D(0, DtoR(0), 0), CVector3D(0.01f, 0.01f, 0.01f)), "Archer2");

	Player::GetInstance()->m_StateAI.ChangeState(PlayerState::IdleState);

	//new FireElemental(Transform(CVector3D(0, 0, 10), CVector3D(0, DtoR(0), 0), CVector3D::one), "FireElemental");

	new Geometry(Transform(CVector3D::zero, CVector3D::zero, CVector3D(10,10,10)), StarterAsset::Plate);

	Map* map = new Map();

	map->GetDungeonData();

	CVector3D player_pos;

	if (DungeonMarker::GetRandomDungeonPos(&map->GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		CVector3D pos(player_pos.x * TILE_SIZE, 0, player_pos.z * TILE_SIZE);
		
		Player::GetInstance()->m_Transform.position = pos;
	};

	new GUI();

	

	if (DungeonMarker::GetRandomDungeonPos(&map->GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		
		new Geometry(Transform(player_pos * TILE_SIZE, CVector3D::zero, CVector3D::one), StarterAsset::Cone);

	};
}

void GameTestState::Execute()
{
}

void GameTestState::Exit()
{
}
