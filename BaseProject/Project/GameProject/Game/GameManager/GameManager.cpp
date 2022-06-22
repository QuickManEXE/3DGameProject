#include "GameManager.h"
#include"State/TitleState.h"
#include"State/OPState.h"
#include"State/Field1State.h"
#include"State/Field1EndState.h"
#include"State/BossState.h"
#include"State/GameClearState.h"
#include"State/GameOverState.h"
#include"State/GameTestState.h"
#include"State/BeginGameState.h"
#include"State/TutorialState.h"
#include"State/InitGameState.h"
#include"State/ExecuteState.h"
#include"State/EndGameState.h"

//GameManager* GameManager::mp_Instance = nullptr;

GameManager::GameManager() : Base(UpdatePriority::eUp_Manager,"GameManager")
{
	srand((unsigned int)time(NULL));

	SetProtect(true);

	m_Render.RegistRender(this, (RenderFunc)&GameManager::Render,RenderPriority::eRd_Manager,"GameManager");

	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&GameManager::CollisionCheck,
		CollisionPriority::eCol_Manager, "GameManager");

	m_StateAI.state_vector.push_back(new TitleState(this));
	m_StateAI.state_vector.push_back(new OPState(this));
	m_StateAI.state_vector.push_back(new Field1State(this));
	m_StateAI.state_vector.push_back(new Field1EndState(this));
	m_StateAI.state_vector.push_back(new BossState(this));
	m_StateAI.state_vector.push_back(new GameClearState(this));
	m_StateAI.state_vector.push_back(new GameOverState(this));
	m_StateAI.state_vector.push_back(new GameTestState(this));
	m_StateAI.state_vector.push_back(new BeginGameState(this));
	m_StateAI.state_vector.push_back(new TutorialState(this));
	m_StateAI.state_vector.push_back(new InitGameState(this));
	m_StateAI.state_vector.push_back(new ExecuteState(this));
	m_StateAI.state_vector.push_back(new EndGameState(this));

	m_StateAI.stateMachine = new StateMachine<GameManager>;

	m_StateAI.ChangeState(GameManagerState::TitleState);

}

void GameManager::Update()
{
	m_StateAI.StateUpdate();
}

void GameManager::Render()
{
	m_StateAI.StateRender();
}

void GameManager::CollisionCheck(CollisionTask* task)
{
	m_StateAI.StateCollisionCheck(task);
}

void GameManager::Destory()
{
	//消すもの
	//敵
	auto tasks = TaskManager::GetInstance()->FindTasks("Enemy");
	for (auto task : tasks) {
		task->SetKill();
	}

	tasks = TaskManager::GetInstance()->FindTasks("EnemySquad");
	for (auto task : tasks) {
		task->SetKill();
	}

	//ドア
	tasks = TaskManager::GetInstance()->FindTasks("Entrance");
	for (auto task : tasks) {
		task->SetKill();
	}

	//アイテム
	tasks = TaskManager::GetInstance()->FindTasks("Item");
	for (auto task : tasks) {
		task->SetKill();
	}


	//アイテムチェスト
	tasks = TaskManager::GetInstance()->FindTasks("ItemChest");
	for (auto task : tasks) {
		task->SetKill();
	}
}
