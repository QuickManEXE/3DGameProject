#pragma once
#include"../GameManager.h"

class GameOverState : public State<GameManager> {
private:

	//後ろの画像
	CImage m_back_img;
	//ゲームオーバーロゴ
	CImage m_title_logo;
	//ロゴの座標
	CVector2D title_pos;
	//ターゲットの座標
	CVector2D target_pos;
	
public:
	GameOverState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();
};