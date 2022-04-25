#include"GameOverState.h"
#include"../../../SoundResource.h"

GameOverState::GameOverState(GameManager* owner) : State(owner)
{
	m_back_img = COPY_RESOURCE("GameOverBack", CImage);
	m_back_img.SetSize(1280, 720);

	m_title_logo = COPY_RESOURCE("TitleLogo", CImage);
	title_pos = CVector2D(150, 50);
}

void GameOverState::Enter()
{
	TaskManager::GetInstance()->AllKill();
	SOUND(SoundResource::BGM_LostPlace.c_str())->Stop();
	SOUND(SoundResource::BGM_BossBattle.c_str())->Stop();
	SOUND(SoundResource::BGM_GAMEOVER.c_str())->Play();
}

void GameOverState::Execute()
{
}

void GameOverState::Exit()
{
}

void GameOverState::Render()
{
	m_back_img.Draw();

	m_title_logo.SetRect(0, 384, 1088, 576);
	m_title_logo.SetSize(1088, 192);
	m_title_logo.SetPos(title_pos);
	m_title_logo.Draw();
}
