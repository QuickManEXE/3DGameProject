#include "GameClearState.h"
#include"../../../SoundResource.h"

GameClearState::GameClearState(GameManager* owner) :  State(owner)
{
	m_back_img = COPY_RESOURCE("GameClearBack", CImage);
	m_back_img.SetSize(1280, 720);

	m_title_logo = COPY_RESOURCE("TitleLogo", CImage);
	title_pos = CVector2D(150, 50);
}

void GameClearState::Enter()
{
	TaskManager::GetInstance()->AllKill();
	SOUND(SoundResource::BGM_BossBattle.c_str())->Stop();
	SOUND(SoundResource::BGM_Fanfare.c_str())->Play();
}

void GameClearState::Execute()
{
}

void GameClearState::Exit()
{
}

void GameClearState::Render()
{
	m_back_img.Draw();

	m_title_logo.SetRect(1152,384, 2240, 576);
	m_title_logo.SetSize(1088, 192);
	m_title_logo.SetPos(title_pos);
	m_title_logo.Draw();
}
