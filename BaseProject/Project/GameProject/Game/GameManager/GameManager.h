#pragma once
#include"../../Base/Base.h"
#include"../../Singleton/Singleton.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticObject.h"
#include"../DungeonEventManager/DungeonEventManager.h"

//ゲームイベントを起こすマネージャー
class GameManager : public Base,public Singleton<GameManager>{
public:
	friend class Singleton<GameManager>;
protected:
	GameManager();
public:
	enum class GameManagerState {
		TitleState,
		OpeningState,
		Field1State,
		Field1EndState,//道が開かれる
		BossBattle,
		GameClearState,
		GameOverState,
		GameTestState,//デバッグ用
		BeginGameState,
		TutorialState,//ゲーム説明シーン
		InitGameState,//ゲームの初期化シーン
		ExecuteState,//ゲーム中のシーン
		EndGameState,//ゲーム終了のシーン
	};

	StateAI<GameManager, GameManagerState> m_StateAI;
private:
	CollisionTask m_Col;
	
	RenderTask m_Render;
public:
	//イベント用の敵カウント
	int m_EnemyCount;

	StaticObject* m_Rock;

	//現在到達しているダンジョンの階層の数値
	int m_CurrentDungeonNum;
	//ゲームクリアのダンジョンの階層の数値
	int m_ClearDungeonNum;

	DungeonEventManager m_event_manager;


	//現在の時間
	float m_current_game_time;
	//ゲームのタイムリミット
	float m_time_limit;

public:
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
	void Destory();
};