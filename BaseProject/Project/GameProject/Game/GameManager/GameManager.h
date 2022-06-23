#pragma once
#include"../../Base/Base.h"
#include"../../Singleton/Singleton.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticObject.h"
#include"../DungeonEventManager/DungeonEventManager.h"

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
		TutorialState,//�Q�[�������V�[��
		InitGameState,//�Q�[���̏������V�[��
		ExecuteState,//�Q�[�����̃V�[��
		EndGameState,//�Q�[���I���̃V�[��
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

	DungeonEventManager m_event_manager;


	//���݂̎���
	float m_current_game_time;
	//�Q�[���̃^�C�����~�b�g
	float m_time_limit;

public:
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
	void Destory();
};