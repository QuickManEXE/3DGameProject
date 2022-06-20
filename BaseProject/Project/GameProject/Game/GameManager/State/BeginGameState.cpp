#include"BeginGameState.h"
#include"../../Player/Player.h"
#include"../../Geometry.h"
#include"../../SkyBox.h"
#include"../../Map.h"
#include"../../GUI.h"
#include"../../FireElemental/FireElemental.h"
#include"../../Object/Goal.h"


BeginGameState::BeginGameState(GameManager* owner) : State(owner)
{
	
}

void BeginGameState::Enter()
{
	owner->m_CurrentDungeonNum++;

	//ƒQ[ƒ€ƒNƒŠƒA‚©‚Ç‚¤‚©
	if (owner->m_CurrentDungeonNum > owner->m_ClearDungeonNum) {

		owner->m_StateAI.ChangeState(GameManager::GameManagerState::GameClearState);

	}
	Map::Instance().CreateDungeon(CVector2D(MAP_WIDTH, MAP_HEIGHT), CVector2D(4, 4), CVector2D(MAP_WIDTH / 2, MAP_HEIGHT / 2), 10);

	GUI::Instance().InitMiniMap(Map::Instance().GetDungeonData());

	CVector3D player_pos;

	if (DungeonMarker::GetRandomDungeonPos(&Map::Instance().GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		CVector3D pos(player_pos.x * TILE_SIZE, 0, player_pos.z * TILE_SIZE);

		Player::GetInstance()->m_Transform.position = pos;
	};

	if (DungeonMarker::GetRandomDungeonPos(&Map::Instance().GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);

		Goal::Instance().m_Transform = Transform(player_pos * TILE_SIZE + CVector3D(0, -1, 0), CVector3D::zero, CVector3D::one);

	};
}

void BeginGameState::Execute()
{
	
}

void BeginGameState::Exit()
{
}
