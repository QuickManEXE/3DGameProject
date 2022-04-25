#include "GUI.h"
#include"Player/Player.h"

float GUI::m_time_ang;

GUI::GUI() :Base(UpdatePriority::eUp_HUD, "UI"), m_text("しょかきうたげ（無料版）", 32)
{
	SetProtect(true);

	// 描画タスクに登録
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
	//各種機能を無効に
	//カリング無効
	glDisable(GL_CULL_FACE);
	//ライティング無効
	glDisable(GL_LIGHTING);
	//デプステスト無効
	glDisable(GL_DEPTH_TEST);
	//
	//正射投影の行列を作成
	//※2D描画用の設定
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();
	CMatrix pvm = mProj * CMatrix::MTranselate(pos) * CMatrix::MScale(size, size, 1);
	glPushMatrix();
	glLoadMatrixf(pvm.f);
	//色設定
	glColor3f(color.r, color.g, color.b);
	//描画開始
	//http://web.wakayama-u.ac.jp/~wuhy/GSS/02.html
	glBegin(GL_TRIANGLE_FAN);
	//円の最大分割数
	const int cut = 180;
	//ループ回数
	int loop = cut * ang / (M_PI * 2);
	//中心設定
	glVertex2f(0, 0);
	//円弧を描く
	for (int i = 0; i < loop; ++i) {
		float s = sin(M_PI * 2 * i / cut);
		float c = -cos(M_PI * 2 * i / cut);
		glVertex2f(s, c);
	}
	//最後
	float s = sin(ang);
	float c = -cos(ang);
	glVertex2f(s, c);
	glEnd();
	//設定を元に戻す
	glPopMatrix();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void GUI::DrawDubugInfo()
{
#ifdef _DEBUG

	Transform player_trans = Player::GetInstance()->m_Transform;

	m_text.Draw(50, 25, 0, 0, 1, "座標 x:%.0f y:%.0f z:%.0f",
		player_trans.position.x, player_trans.position.y, player_trans.position.z);
	m_text.Draw(50, 50, 0, 1, 0, "移動ベクトル = %f", player_trans.m_pos_vec.Length());

	m_text.Draw(50, 100, 1, 0, 1, "Wキー：前進");
	m_text.Draw(50, 125, 1, 0, 1, "Aキー：左移動");
	m_text.Draw(50, 150, 1, 0, 1, "Sキー：後退");
	m_text.Draw(50, 175, 1, 0, 1, "Dキー：右移動");

	m_text.Draw(50, 200, 1, 0, 1, "SHIFTキー（ホールド）：走る");

	m_text.Draw(50, 300, 1, 0, 1, "マウスL：弓矢をうつ");
	m_text.Draw(50, 350, 1, 0, 1, "マウスR：ワイヤー発射");

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

	//HPの枠
	m_gui.SetRect(0, 32, 192, 64);
	m_gui.SetSize(hp_size);
	m_gui.SetPos(hp_pos);
	m_gui.Draw();

	//HPのバー
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

	//HPのバー
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