#pragma once
#include"../../Base/Base.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticObject.h"

//ゲームイベントを起こすマネージャー
class GameManager : public Base {
private:
	GameManager();
	static GameManager* mp_Instance;
public:
	static GameManager* GetInstance();
	static void CreateInstance();
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
	};

	StateAI<GameManager, GameManagerState> m_StateAI;
private:
	CollisionTask m_Col;
	
	RenderTask m_Render;
public:
	//イベント用の敵カウント
	int m_EnemyCount;

	StaticObject* m_Rock;
public:
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
};