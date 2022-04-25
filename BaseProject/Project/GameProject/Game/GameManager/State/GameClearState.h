#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"

class GameClearState : public State<GameManager> {
private:

	//���̉摜
	CImage m_back_img;
	//�Q�[���N���A�[���S
	CImage m_title_logo;
	//�^�C�g���̍��W
	CVector2D title_pos;
	//�^�[�Q�b�g�̍��W
	CVector2D target_pos;
	//�^�C�g���̃v�b�V���X�^�[�g��\�����邩�ǂ���
	bool m_is_start;
	//push start�̃A�j���[�V�����̕ϐ�
	int anim_x;

public:
	GameClearState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();
};