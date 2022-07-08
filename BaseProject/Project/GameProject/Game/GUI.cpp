#include "GUI.h"
#include"Player/Player.h"
#include"Map.h"
#include"GameManager\GameManager.h"
#include"Object\Goal.h"

float GUI::m_time_ang;

GUI::GUI() :Base(UpdatePriority::eUp_HUD, "UI"), m_text("ＭＳゴシック", 32)
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

void GUI::Update()
{
	if (!GameManager::Instance().m_StateAI.IsCurrentState(GameManager::GameManagerState::ExecuteState))return;
	UpdateMiniMap(Map::Instance().GetDungeonData());
}

void GUI::Draw()
{
	
	DrawHP();
	DrawIcon();
	DrawAimTarget();
	DrawAimTarget2();
	//DrawMiniMap(m_DungeonData_For_MiniMap);
	DrawGameInfo();
	DrawTimeLimit();
	DrawHookIcon();
	DrawDubugInfo();
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

void GUI::InitMiniMap(DungeonMarker::DungeonData _DungeonData_For_MiniMap)
{
	m_DungeonData_For_MiniMap = _DungeonData_For_MiniMap;

	int dungeon_hegiht = m_DungeonData_For_MiniMap.m_tile.size();
	int dungeon_width = m_DungeonData_For_MiniMap.m_tile[0].size();

	for (int i=0; i < dungeon_hegiht; i++ ) {

		for (int j=0; j < dungeon_width; j++) {

			m_DungeonData_For_MiniMap.m_tile[i][j] = -1;
		}
	}

}

void GUI::DrawMiniMap(DungeonMarker::DungeonData _DungeonData_For_MiniMap)
{
	if (!GameManager::Instance().m_StateAI.IsCurrentState(GameManager::GameManagerState::ExecuteState))return;

	int size = 6;
	CVector2D p(1100, 150);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (_DungeonData_For_MiniMap.m_tile[i][j] <= (int)DungeonMarker::TileType::inside_wall_id) {
				/*m_gui.SetRect(0, 401, 20, 421);
				m_gui.SetSize(size, size);
				m_gui.SetPos(p + CVector2D(size * j, size * i));
				m_gui.Draw();*/
				CVector2D pos(p + CVector2D(size * j, size * i));
				CVector2D t_size(size, size);
				Utility::DrawQuad(pos, t_size, CColorRGBA(1, 0, 0, 1));
			}
		}
	}


	int ix, iy;
	Map::Instance().GetTip(Player::GetInstance()->m_Transform.position, &ix, &iy);
	CVector2D pos(p + CVector2D(size * ix, size * iy));
	CVector2D t_size(size, size);
	Utility::DrawQuad(pos, t_size, CColorRGBA(1, 1, 0, 1));
	/*m_gui.SetRect(20, 401, 40, 421);
	m_gui.SetSize(size, size);
	m_gui.SetPos(p + CVector2D(size * ix, size * iy));
	m_gui.Draw();*/

	
	Map::Instance().GetTip(Goal::Instance().m_Transform.position, &ix, &iy);
	if (_DungeonData_For_MiniMap.m_tile[iy][ix] != -1) {
		pos = (p + CVector2D(size * ix, size * iy));
		Utility::DrawQuad(pos, t_size, CColorRGBA(0, 0, 1, 1));
	}
	
	/*m_gui.SetRect(20, 401, 40, 421);
	m_gui.SetSize(size, size);
	m_gui.SetPos(p + CVector2D(size * ix, size * iy));
	m_gui.Draw();*/

}

void GUI::UpdateMiniMap(DungeonMarker::DungeonData _DungeonData_For_MiniMap)
{
	Player::GetInstance()->m_Transform.position;
	int ix, iy;
	Map::Instance().GetTip(Player::GetInstance()->m_Transform.position, &ix, &iy);

	int dungeon_hegiht = m_DungeonData_For_MiniMap.m_tile.size();
	int dungeon_width = m_DungeonData_For_MiniMap.m_tile[0].size();

	CRect update_rect(max(0, ix - 2), max(0, iy - 2), min(dungeon_width, ix + 2), min(dungeon_hegiht, iy + 2));

	for (int i = update_rect.m_top ; i < update_rect.m_bottom; i++) {

		for (int j = update_rect.m_left; j < update_rect.m_right; j++) {

			m_DungeonData_For_MiniMap.m_tile[i][j] = _DungeonData_For_MiniMap.m_tile[i][j];
		}
	}

}

void GUI::DrawGameInfo() {

#ifdef _DEBUG

	m_text.Draw(1000, 20, 1, 0, 1, "階:%d/%d", GameManager::Instance().m_CurrentDungeonNum,GameManager::Instance().m_ClearDungeonNum);
	
#endif // _DEBUG
}

void GUI::DrawTimeLimit()
{
	if (!GameManager::Instance().m_StateAI.IsCurrentState(GameManager::GameManagerState::ExecuteState))return;

	int second =  (int)GameManager::Instance().m_current_game_time % 60;
	int  min = (int)GameManager::Instance().m_current_game_time / 60;

	
	m_text.Draw(1100, 50, 1, 0, 1, "TIME %d:%02d",min,second);
	//m_text.Draw(1100, 100, 1, 0, 1, "階: %d / %d", GameManager::Instance().m_CurrentDungeonNum, GameManager::Instance().m_ClearDungeonNum);

}

void GUI::DrawHookIcon()
{
	CVector2D hook_pos(16 + 32, SCREEN_HEIGHT - 200);

	for (int i = 0; i < Player::GetInstance()->m_HookNum; i++) {
		m_hook_icon.SetPos(hook_pos + CVector2D(0, 64 * i));
		m_hook_icon.Draw();
	}
}
