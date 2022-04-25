#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"

class GameClearState : public State<GameManager> {
private:

	//後ろの画像
	CImage m_back_img;
	//ゲームクリアーロゴ
	CImage m_title_logo;
	//タイトルの座標
	CVector2D title_pos;
	//ターゲットの座標
	CVector2D target_pos;
	//タイトルのプッシュスタートを表示するかどうか
	bool m_is_start;
	//push startのアニメーションの変数
	int anim_x;

public:
	GameClearState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();
};