#include "GUI.h"
#include"Player/Player.h"

float GUI::m_time_ang;

GUI::GUI() :Base(UpdatePriority::eUp_HUD, "UI"), m_text("���傩���������i�����Łj", 32)
{
	SetProtect(true);

	// �`��^�X�N�ɓo�^
	m_Draw.RegistRender(this, (RenderFunc)&GUI::Draw, RenderPriority::eRd_HUD, "GUI");

	m_target_icon = COPY_RESOURCE("TargetIcon", CImage);
	m_target_icon.SetCenter(32, 32);
	m_target_icon.SetSize(64, 64);

	m_lookon_icon = COPY_RESOURCE("LookonIcon", CImage);
	m_lookon_icon.SetCenter(32, 32);
	m_lookon_icon.SetSize(64, 64);


	m_hook_icon = COPY_RESOURCE("hook_icon", CImage);
	m_hook_icon.SetCenter(25,25);
	m_hook_icon.SetSize(50, 50);

	m_arrow_bow_icon = COPY_RESOURCE("arrow_bow_icon", CImage);
	m_arrow_bow_icon.SetCenter(64, 64);
	m_arrow_bow_icon.SetSize(128, 128);

	m_gui = COPY_RESOURCE("GUI", CImage);
	
	m_now_hp = 100;
}

void GUI::DrawCircle(const CVector2D& pos, const float size, const float ang, const CVector4D& color)
{
	//�e��@�\�𖳌���
	//�J�����O����
	glDisable(GL_CULL_FACE);
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glDisable(GL_DEPTH_TEST);
	//
	//���˓��e�̍s����쐬
	//��2D�`��p�̐ݒ�
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();
	CMatrix pvm = mProj * CMatrix::MTranselate(pos) * CMatrix::MScale(size, size, 1);
	glPushMatrix();
	glLoadMatrixf(pvm.f);
	//�F�ݒ�
	glColor3f(color.r, color.g, color.b);
	//�`��J�n
	//http://web.wakayama-u.ac.jp/~wuhy/GSS/02.html
	glBegin(GL_TRIANGLE_FAN);
	//�~�̍ő啪����
	const int cut = 180;
	//���[�v��
	int loop = cut * ang / (M_PI * 2);
	//���S�ݒ�
	glVertex2f(0, 0);
	//�~�ʂ�`��
	for (int i = 0; i < loop; ++i) {
		float s = sin(M_PI * 2 * i / cut);
		float c = -cos(M_PI * 2 * i / cut);
		glVertex2f(s, c);
	}
	//�Ō�
	float s = sin(ang);
	float c = -cos(ang);
	glVertex2f(s, c);
	glEnd();
	//�ݒ�����ɖ߂�
	glPopMatrix();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void GUI::DrawDubugInfo()
{
#ifdef _DEBUG

	Transform player_trans = Player::GetInstance()->m_Transform;

	m_text.Draw(50, 25, 0, 0, 1, "���W x:%.0f y:%.0f z:%.0f",
		player_trans.position.x, player_trans.position.y, player_trans.position.z);
	m_text.Draw(50, 50, 0, 1, 0, "�ړ��x�N�g�� = %f", player_trans.m_pos_vec.Length());

	m_text.Draw(50, 100, 1, 0, 1, "W�L�[�F�O�i");
	m_text.Draw(50, 125, 1, 0, 1, "A�L�[�F���ړ�");
	m_text.Draw(50, 150, 1, 0, 1, "S�L�[�F���");
	m_text.Draw(50, 175, 1, 0, 1, "D�L�[�F�E�ړ�");

	m_text.Draw(50, 200, 1, 0, 1, "SHIFT�L�[�i�z�[���h�j�F����");

	m_text.Draw(50, 300, 1, 0, 1, "�}�E�XL�F�|�������");
	m_text.Draw(50, 350, 1, 0, 1, "�}�E�XR�F���C���[����");

#endif // _DEBUG
}

void GUI::DrawAimTarget()
{
	if (Player::GetInstance()->m_StateAI.IsCurrentState(PlayerState::ArrowAimState)) {

		CVector3D camera_at = CCamera::GetCurrent()->GetAt();
		CVector2D screen_pos;
		Utility::WorldToScreen(&screen_pos, camera_at);

		m_target_icon.SetPos(screen_pos);
		m_target_icon.Draw();

	}

}

void GUI::DrawAimTarget2()
{
	if (Player::GetInstance()->m_StateAI.IsCurrentState(PlayerState::WireAimState)) {

		if (Player::GetInstance()->m_IsLookOn) {

			CVector3D camera_at = CCamera::GetCurrent()->GetAt();
			CVector2D screen_pos;
			Utility::WorldToScreen(&screen_pos, camera_at);

			m_lookon_icon.SetPos(screen_pos);
			m_lookon_icon.Draw();

		}
		else {

			CVector3D camera_at = CCamera::GetCurrent()->GetAt();
			CVector2D screen_pos;
			Utility::WorldToScreen(&screen_pos, camera_at);

			m_target_icon.SetPos(screen_pos);
			m_target_icon.Draw();
		}
	}
}

void GUI::DrawHP()
{
	
	float p_hp = Player::GetInstance()->m_HitPoint;
	float p_hp_max = 100;

	m_now_hp += (p_hp - m_now_hp) * DELTA;

	float per = max(m_now_hp / p_hp_max,0.0f);

	CVector2D hp_pos(16+64, SCREEN_HEIGHT - 128 -128);
	CVector2D hp_size(192, 32);

	//HP�̘g
	m_gui.SetRect(0, 32, 192, 64);
	m_gui.SetSize(hp_size);
	m_gui.SetPos(hp_pos);
	m_gui.Draw();

	//HP�̃o�[
	m_gui.SetRect(0, 0, 192*per, 32);
	m_gui.SetSize(192*per, 32);
	m_gui.SetPos(hp_pos);
	m_gui.Draw();
}

void GUI::DrawIcon()
{
	CRect icon_rect(0, 64, 192, 192);
	CVector2D icon_pos(16+64, SCREEN_HEIGHT - 64 - 128);
	CVector2D icon_size(192,128);

	//HP�̃o�[
	m_gui.SetRect(0,64,192,192);
	m_gui.SetSize(icon_size);
	m_gui.SetPos(icon_pos);
	m_gui.Draw();
}



void GUI::Update()
{	
	
}

void GUI::Draw()
{

	DrawHP();
	DrawIcon();
	DrawAimTarget();
	DrawAimTarget2();
	
	CVector2D hook_pos(16+32, SCREEN_HEIGHT - 200);
		
	for (int i = 0; i < Player::GetInstance()->m_HookNum ; i++) {
		m_hook_icon.SetPos(hook_pos + CVector2D(0,64*i));
		m_hook_icon.Draw();
	}

	DrawDubugInfo();
}