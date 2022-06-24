#include "ExecuteState.h"
#include"../../Player/Player.h"
#include"../../Geometry.h"
#include"../../SkyBox.h"
#include"../../Map.h"
#include"../../GUI.h"
#include"../../FireElemental/FireElemental.h"
#include"../../Object/Goal.h"
#include"../../Object/Entrance.h"


ExecuteState::ExecuteState(GameManager* owner) : State(owner)
{

}

void ExecuteState::Enter()
{

	
	owner->m_CurrentDungeonNum++;

	//ゲームクリアかどうか
	if (owner->m_CurrentDungeonNum > owner->m_ClearDungeonNum) {

		owner->m_StateAI.ChangeState(GameManager::GameManagerState::BossBattle);
		return;
	}

	//ダンジョンデータの生成
	Map::Instance().CreateDungeon(CVector2D(MAP_WIDTH, MAP_HEIGHT), CVector2D(4, 4), CVector2D(MAP_WIDTH / 2, MAP_HEIGHT / 2), 10);


	//プレイヤーのステートを通常状態にする
	Player::GetInstance()->m_StateAI.ChangeState(PlayerState::IdleState);
	//プレイヤーをランダムな位置に配置
	CVector3D player_pos;

	if (DungeonMarker::GetRandomDungeonRoomPos(&Map::Instance().GetDungeonData(), &player_pos, Map::Instance().GetDungeonData().start_room_num)) {

		Player::GetInstance()->m_Transform.position = player_pos * TILE_SIZE;
	};

	//プレイヤーをランダムな位置に配置
	CVector3D goal_pos;
	if (DungeonMarker::GetRandomDungeonRoomPos(&Map::Instance().GetDungeonData(), &goal_pos, Map::Instance().GetDungeonData().goal_room_num)) {

		Goal::Instance().m_Transform = Transform(goal_pos * TILE_SIZE + CVector3D(0, -TILE_SIZE/2.0f, 0), CVector3D::zero, CVector3D::one);
	};

	//扉の出現
	SetEntrance();

	//ミニマップデータの初期化
	GUI::Instance().InitMiniMap(Map::Instance().GetDungeonData());

	//イベントデータの登録
	DungeonEventManager::Instance().SetEvent(Map::Instance().GetDungeonData(), TILE_SIZE);

	//時間の設定
	owner->m_current_game_time = 60.0f * 5.0f;
	owner->m_time_limit = 60.0f * 5.0f;

	SOUND("BGM_LostPlace")->Play(true); 

}

void ExecuteState::Execute()
{
	DungeonEventManager::Instance().Run();

	owner->m_current_game_time = max(0, owner->m_current_game_time - DELTA);

	if (owner->m_current_game_time <= 0) {

		owner->m_StateAI.ChangeState(GameManager::GameManagerState::GameOverState);

	}
}

void ExecuteState::Exit()
{
	owner->Destory();

	DungeonEventManager::Instance().RemoveEvents();
}

void ExecuteState::SetEntrance()
{
	DungeonMarker::DungeonData d_data = Map::Instance().GetDungeonData();
	DungeonMarker::RoomDatas2 r_data = d_data.m_rooms2;
	DungeonMarker::EntranceDatas e_data = d_data.m_entrances;

	for (int i = 0; i < e_data.size(); i++) {

		const auto& entrance = e_data[i];
		CVector3D pos = entrance.position;

		CVector3D rot(0, 0, 0);
		switch (entrance.dir)
		{
		case DungeonMarker::DirectionType::direction_east:
		case DungeonMarker::DirectionType::direction_west:
			rot.y = DtoR(90);
			break;
		case DungeonMarker::DirectionType::direction_north:
		case DungeonMarker::DirectionType::direction_south:
			break;
		default:
			break;
		}

		Entrance* e = new Entrance(entrance.parent_room_num,Transform(pos * TILE_SIZE + CVector3D(0,-3,0), rot, CVector3D(3, 3, 1)));
		
	}

}


void ExecuteState::CollisionCheck(CollisionTask* _task)
{

	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		CVector3D pos = c->m_Transform.position;

		DungeonEventManager::Instance().CheckCollision(pos + CVector3D(0, 2.0f, 0));
	}

}