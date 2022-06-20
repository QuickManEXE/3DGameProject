#pragma once
#include"../../Base/Base.h"
#include"../../Singleton/Singleton.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticObject.h"


//�Q�[���C�x���g���N�����}�l�[�W���[
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
		Field1EndState,//�����J�����
		BossBattle,
		GameClearState,
		GameOverState,
		GameTestState,//�f�o�b�O�p
		BeginGameState,
	};

	StateAI<GameManager, GameManagerState> m_StateAI;
private:
	CollisionTask m_Col;
	
	RenderTask m_Render;
public:
	//�C�x���g�p�̓G�J�E���g
	int m_EnemyCount;

	StaticObject* m_Rock;

	//���ݓ��B���Ă���_���W�����̊K�w�̐��l
	int m_CurrentDungeonNum;
	//�Q�[���N���A�̃_���W�����̊K�w�̐��l
	int m_ClearDungeonNum;

public:
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
};