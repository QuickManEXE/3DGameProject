#include "InitGameState.h"
#include"../../Player/Player.h"
#include"../../Geometry.h"
#include"../../SkyBox.h"
#include"../../Map.h"
#include"../../GUI.h"
#include"../../FireElemental/FireElemental.h"
#include"../../Object/Goal.h"
#include"../../Enemy/Enemy.h"
#include"../../Object/Entrance.h"


InitGameState::InitGameState(GameManager* owner) : State(owner)
{
}

void InitGameState::Enter()
{
	//ダンジョン開始の初期化処理
	//目標の階層と開始の階層を設定
	owner->m_CurrentDungeonNum = 0;
	owner->m_ClearDungeonNum = 1;

	//スカイボックスの生成
	new SkyBox();

	//ダンジョンインスタンスの生成
	Map::Build();
	
	//UIインスタンスの生成
	GUI::Build();

	//ダンジョンイベントシステムのインスタンスの生成
	DungeonEventManager::Build();
	
	//プレイヤーの生成
	new Player(Transform(CVector3D(0, 0, 0), CVector3D(0, DtoR(0), 0), CVector3D(0.01f, 0.01f, 0.01f)), "Archer2");
	
	Goal::Build();

	//ゲームの実行状態に遷移
	owner->m_StateAI.ChangeState(GameManager::GameManagerState::ExecuteState);
}

void InitGameState::Execute()
{
}

void InitGameState::Exit()
{
}
