#include "RopeIdleState.h"



RopeIdleState::RopeIdleState(Rope* owner) : State(owner)
{
}

void RopeIdleState::Enter()
{
}

void RopeIdleState::Execute()
{
	//ロープの角度から行列を求める
	CMatrix m = CMatrix::MRotation(owner->m_Transform.rotation);

	//ロープの終端の位置を決定
	//owner->lineE = owner->lineS + -m.GetUp() * owner->rope_length;

	//プレイヤーが捕まっている位置を更新
	//owner->m_conect_point = owner->lineS + -m.GetUp() * owner->conect_dir;

	
}

void RopeIdleState::Exit()
{
}

