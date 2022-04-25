#pragma once
#include"../../Base/Base.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticObject.h"

//�Q�[���C�x���g���N�����}�l�[�W���[
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
		Field1EndState,//�����J�����
		BossBattle,
		GameClearState,
		GameOverState,
		GameTestState,//�f�o�b�O�p
	};

	StateAI<GameManager, GameManagerState> m_StateAI;
private:
	CollisionTask m_Col;
	
	RenderTask m_Render;
public:
	//�C�x���g�p�̓G�J�E���g
	int m_EnemyCount;

	StaticObject* m_Rock;
public:
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
};