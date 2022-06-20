#include"TitleState.h"
#include"../../../SoundResource.h"
#include"../../../Effect/Effect2D.h"
#include"../../SkyBox.h"
#include"../../GUI.h"
#include"../../AnimObject.h"
#include"../../NonAnimObject.h"

//タイトルのステート
TitleState::TitleState(GameManager* owner) : State(owner)
{

}

void TitleState::Enter()
{
	m_title_logo = COPY_RESOURCE("TitleLogo", CImage);
	 
	old_cam_transform = Transform(CVector3D(0.311612,5.460681,-13.124462), CVector3D(-0.183602,0.372500,0.000000), CVector3D::one);
	new_cam_transform = old_cam_transform;

	Camera::GetInstance()->SetCamera(this);

	title_pos = CVector2D(128, -250);

	target_pos = CVector2D(128, 100);

	anim_x = 0;

	SOUND(SoundResource::BGM_Title.c_str())->Play();

	new SkyBox();

	NonAnimObject* field = new NonAnimObject( UpdatePriority::eUp_Field,"OpeningMountain",RenderPriority::eRd_Field);
	field->SetTransform(Transform(CVector3D::zero, CVector3D::zero, CVector3D::one));

	AnimObject* player = new AnimObject(UpdatePriority::eUp_Player,"OpeningPlayer",RenderPriority::eRd_Player);
	player->SetTransform(Transform(CVector3D(1.540770, 3.940763, -12.363358), CVector3D(-0.064602, 0.270000, 0.000000), CVector3D(0.01f, 0.01f, 0.01f)));
	
	NonAnimObject* map = new NonAnimObject(UpdatePriority::eUp_Character,"TreasureMap",RenderPriority::eRd_Charactor);
	
	map->SetTransform(Transform(CVector3D(5.077944, 5.053736, -11.395449), CVector3D(-0.114602, 0.140000, 0.000000), CVector3D(0.15f, 0.15f, 0.15f)));
	map->SetParent(player->GetModel(), 13);
	new Effect2D("Transition2", CVector2D::zero, CVector2D::zero, CVector2D::zero, 0.0f, 0.0f,
		CRect(CVector2D::zero, CVector2D(SCREEN_WIDTH, SCREEN_HEIGHT)),0.0f, 1.0f, -1.0f, 1.0f, 1.0f);


	m_title_state = 0;

	m_t = 0;

	m_t_change = 1.0f;
}

void TitleState::Execute()
{
	UpdateCamMove();

	UpdateTitle();
}

void TitleState::Exit()
{
	TaskManager::GetInstance()->AllKill();
}

void TitleState::UpdateTitle()
{
	m_t = min(1.0f, m_t + DELTA/m_t_change);
	cam_transform.position = old_cam_transform.position * (1.0f - m_t) + new_cam_transform.position * m_t;
	cam_transform.rotation = old_cam_transform.rotation * (1.0f - m_t) + new_cam_transform.rotation * m_t;

	switch (m_title_state)
	{
	case 0:
		if (target_pos.y > title_pos.y) {
			float speed = 50.0f;
			title_pos.y += speed * DELTA;
		}
		else {
			m_title_state++;
		}
		//マウスの左ボタンを押したら
		if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {
			//タイトルロゴをスキップする
			title_pos.y = target_pos.y;
		}
		break;
	case 1:
		anim_x = (anim_x >= 5 * 5) ? 0 : ++anim_x;

		//マウスの左ボタンを押したら
		if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {
			m_title_state++;
			m_t = 0.0;
		}
		break;
	case 2:
		old_cam_transform = new_cam_transform;
		new_cam_transform.position = CVector3D(1.502269 ,5.419027, - 12.359076);
		new_cam_transform.rotation = CVector3D(0.748732, 0.124792 ,0.000000);
		m_title_state++;
		break;
	case 3:
		if (m_t >= 1.0f) {
			m_title_state++;
		}
		break;
	case 4:
		old_cam_transform = new_cam_transform; 
		new_cam_transform.position = CVector3D(1.690734, 5.333927, -12.146278);
		new_cam_transform.rotation = CVector3D(0.783565, 0.075115, 0.000000);
		m_t = 0.0f;
		m_t_change = 5.0f;
		new Effect2D("Transition2", CVector2D::zero, CVector2D::zero, CVector2D::zero, 0.0f, 0.0f,
			CRect(CVector2D::zero, CVector2D(SCREEN_WIDTH, SCREEN_HEIGHT)), 0.0f, 0.0f, 0.2f, 6.0f, 1.0f);
		m_title_state++;
		break;
	case 5:
		if (m_t >= 1.0f) {
			SOUND(SoundResource::BGM_Title.c_str())->Stop();

			owner->m_StateAI.ChangeState(GameManager::GameManagerState::GameTestState);
		}
	default:
		break;
	}
}

void TitleState::UpdateCamMove()
{

	if (CInput::GetState(0, CInput::eHold, CInput::eMouseR)) {
		CVector2D mouse_vec = CInput::GetMouseVec();
		new_cam_transform.rotation.y += mouse_vec.x * -0.01f;
		new_cam_transform.rotation.x += mouse_vec.y * 0.01f;
		new_cam_transform.rotation.x = min(DtoR(45), max(DtoR(-45), new_cam_transform.rotation.x));
	}

	//方向キーの入力方向ベクトル
	CVector3D key_dir(0, 0, 0);
	//キャラクターの移動量
	float speed =1.0f;
	
	key_dir = Utility::GetInputKeyDir();

	CMatrix rotMtx = CMatrix::MRotation(cam_transform.rotation);

	//キー入力があれば
	if (key_dir.LengthSq() > 0) {

		//移動処理 回転行列×移動方向
		CVector3D dir = rotMtx * key_dir;
		new_cam_transform.position += dir * speed * DELTA;;
	}


}

void TitleState::Render()
{

	switch (m_title_state)
	{
	case 0:
		RenderLogo();
		break;
	case 1:
		RenderLogo();
		RenderStart();
		break;
	default:
		break;
	}
		
}

void TitleState::RenderLogo()
{
	m_title_logo.SetRect(0, 0, 1088, 256);
	m_title_logo.SetSize(1088, 256);
	m_title_logo.SetPos(title_pos);
	m_title_logo.Draw();
}

void TitleState::RenderStart()
{
	int anim = anim_x / 5;
	m_title_logo.SetRect(0 + 384 * anim, 256, 384 + 384 * anim, 384);
	m_title_logo.SetSize(384, 128);
	m_title_logo.SetPos(CVector2D(SCREEN_WIDTH / 2 - 192, SCREEN_HEIGHT / 2 + 100));
	m_title_logo.Draw();
}

void TitleState::UpdateCamera()
{

	//キャラクターの行列
	CMatrix charactor_matrix;
	//支点の行列
	CMatrix parent_matrix;
	//カメラの行列
	CMatrix cam_matrix;
	//キャラクターの行列
	charactor_matrix = CMatrix::MTranselate(cam_transform.position);
	//支点の行列
	parent_matrix = charactor_matrix * CMatrix::MRotation(cam_transform.rotation);
	//カメラの行列
	cam_matrix = parent_matrix;

	CCamera::GetCurrent()->SetTranseRot(cam_matrix);

	float m_fov=DtoR(60);
	CCamera::GetCurrent()->Perspective(m_fov, (float)CCamera::GetCurrent()->GetWhidth() / (float)CCamera::GetCurrent()->GetHeight(), 0.1, 10000.0);;
}
