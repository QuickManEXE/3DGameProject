#include"GameTestState.h"
#include"../../Player/Player.h"
#include"../../Geometry.h"
#include"../../SkyBox.h"
#include"../../Map.h"
#include"../../GUI.h"
#include"../../FireElemental/FireElemental.h"

GameTestState::GameTestState(GameManager* owner) : State(owner)
{
}

void GameTestState::Enter()
{
	new SkyBox();

	new Player(Transform(CVector3D(0, 10, 0), CVector3D(0, DtoR(0), 0), CVector3D(0.01f, 0.01f, 0.01f)), "Archer2");
	Player::GetInstance()->m_StateAI.ChangeState(PlayerState::IdleState);

	new FireElemental(Transform(CVector3D(0, 0, 10), CVector3D(0, DtoR(0), 0), CVector3D::one), "FireElemental");

	new Geometry(Transform(CVector3D::zero, CVector3D::zero, CVector3D::one), StarterAsset::Plate);

	//new Map();

	new GUI();
}

void GameTestState::Execute()
{
}

void GameTestState::Exit()
{
}
