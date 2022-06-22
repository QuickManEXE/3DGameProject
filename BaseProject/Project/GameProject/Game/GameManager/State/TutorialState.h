#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"

class TutorialState : public State<GameManager>{
private:
	float m_t;

	float m_t_change;

	Transform old_cam_transform;

	Transform new_cam_transform;

	Transform cam_transform;

	//タイトルロゴ
	CImage m_title_logo;
	//タイトルの座標
	CVector2D title_pos;
	//ターゲットの座標
	CVector2D target_pos;

	//push startのアニメーションの変数
	int anim_x;
	//タイトルのステート
	int m_title_state;
public:
	TutorialState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();
};