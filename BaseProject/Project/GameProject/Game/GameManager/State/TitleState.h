#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"

class TitleState : public State<GameManager>,public ICamera {
private:
	float m_t;

	float m_t_change;

	Transform old_cam_transform;

	Transform new_cam_transform;

	Transform cam_transform;
	
	//�^�C�g�����S
	CImage m_title_logo;
	//�^�C�g���̍��W
	CVector2D title_pos;
	//�^�[�Q�b�g�̍��W
	CVector2D target_pos;
	
	//push start�̃A�j���[�V�����̕ϐ�
	int anim_x;
	//�^�C�g���̃X�e�[�g
	int m_title_state;
public:
	TitleState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void UpdateTitle();

	void UpdateCamMove();

	void Render();
	
	void RenderLogo();

	void RenderStart();

	void UpdateCamera()override;
};