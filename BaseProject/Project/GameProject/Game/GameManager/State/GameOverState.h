#pragma once
#include"../GameManager.h"

class GameOverState : public State<GameManager> {
private:

	//���̉摜
	CImage m_back_img;
	//�Q�[���I�[�o�[���S
	CImage m_title_logo;
	//���S�̍��W
	CVector2D title_pos;
	//�^�[�Q�b�g�̍��W
	CVector2D target_pos;
	
public:
	GameOverState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();
};