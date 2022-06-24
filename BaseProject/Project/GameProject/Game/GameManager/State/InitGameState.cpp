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
	//�_���W�����J�n�̏���������
	//�ڕW�̊K�w�ƊJ�n�̊K�w��ݒ�
	owner->m_CurrentDungeonNum = 0;
	owner->m_ClearDungeonNum = 1;

	//�X�J�C�{�b�N�X�̐���
	new SkyBox();

	//�_���W�����C���X�^���X�̐���
	Map::Build();
	
	//UI�C���X�^���X�̐���
	GUI::Build();

	//�_���W�����C�x���g�V�X�e���̃C���X�^���X�̐���
	DungeonEventManager::Build();
	
	//�v���C���[�̐���
	new Player(Transform(CVector3D(0, 0, 0), CVector3D(0, DtoR(0), 0), CVector3D(0.01f, 0.01f, 0.01f)), "Archer2");
	
	Goal::Build();

	//�Q�[���̎��s��ԂɑJ��
	owner->m_StateAI.ChangeState(GameManager::GameManagerState::ExecuteState);
}

void InitGameState::Execute()
{
}

void InitGameState::Exit()
{
}
