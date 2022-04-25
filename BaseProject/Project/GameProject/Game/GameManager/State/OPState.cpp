#include "OPState.h"
#include"../../SkyBox.h"
#include"../../Geometry.h"
#include"../../Player/Player.h"
#include"../../Camera.h"
#include"../../../SoundResource.h"
#include"../../StaticObject.h"

CVector3D OPState::m_cam_pos_index[m_cam_pos_num_max] = {
	CVector3D(-45,20,25),
	CVector3D(-38,10,46),
	CVector3D(5,10,40),
	CVector3D(0,0,0),
};

OPState::OPState(GameManager* owner) : State(owner)
{
}

void OPState::Enter()
{
	m_title_logo = COPY_RESOURCE("TitleLogo", CImage);

	CreateField();

	Camera::GetInstance()->SetCamera(this);

	SOUND(SoundResource::BGM_LostPlace.c_str())->Play(true);

	m_cam_pos_num = 0;
	t = 0;
	m_now_cam_pos = m_cam_pos_index[m_cam_pos_num];
	m_next_cam_pos = m_cam_pos_index[m_cam_pos_num + 1];

	m_alpha = 0.0f;
}

void OPState::Execute()
{
	m_alpha = min(1.0f, m_alpha + DELTA);
}

void OPState::Exit()
{
}

void OPState::OPUpdate()
{

	//マウスの左ボタンを押したら
	if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {

		t += 1.0f;

	}
	
	if (t >= 1) {

		m_cam_pos_num++;

		if (m_cam_pos_num == m_cam_pos_num_max-1)
		{
			//printf("終わりました\n");
			
			owner->m_StateAI.ChangeState(GameManager::GameManagerState::Field1State);
		}
		else {
			t = 0;

			m_now_cam_pos = m_cam_pos_index[m_cam_pos_num];

			m_next_cam_pos = m_cam_pos_index[m_cam_pos_num + 1];

		}
	}
	else { t += DELTA/5; }

	CVector3D  pos = m_now_cam_pos * (1 - t) + m_next_cam_pos * t;

	CameraAtPos(pos);
}

void OPState::CreateField()
{

	new Geometry(Transform(CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(1,1,1)), "Mountain");

	new SkyBox();

	//邪魔な岩
	owner->m_Rock = new StaticObject(Transform(CVector3D(-46.5, 18, 13), CVector3D(0, 0, 0), CVector3D(12, 12, 12)), "Rock5",4.0f,7.0f);

	new Player(Transform(CVector3D(0, 0, 0), CVector3D(0, DtoR(0), 0), CVector3D(0.01f, 0.01f, 0.01f)), "Archer2");
}

void OPState::UpdateCamera()
{
	OPUpdate();
}

void OPState::CameraAtPos(const CVector3D& pos)
{
	//支点の行列
	CMatrix parent_matrix;
	//カメラの行列
	CMatrix cam_matrix;

	//視点の行列
	parent_matrix = CMatrix::MTranselate(pos);
	//カメラの行列
	cam_matrix = parent_matrix * CMatrix::MTranselate(CVector3D(0,8,8)) * CMatrix::MRotation(CVector3D(DtoR(20), DtoR(180), 0));

	CCamera::GetCurrent()->SetTranseRot(cam_matrix);

	CCamera::GetCurrent()->Perspective(DtoR(60), (float)CCamera::GetCurrent()->GetWhidth() / (float)CCamera::GetCurrent()->GetHeight(), 1.0, 10000.0);;

}

void OPState::Render()
{
	m_title_logo.SetColor(1, 1, 1, m_alpha);
	m_title_logo.SetRect(0, 576, 1792, 832);
	m_title_logo.SetSize(CVector2D(1792, 832-576)*0.5f);
	m_title_logo.SetPos(CVector2D(100,100));
	m_title_logo.Draw();
}
