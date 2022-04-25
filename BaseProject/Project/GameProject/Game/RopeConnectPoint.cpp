#include "RopeConnectPoint.h"
#include"CameraManager.h"

std::vector<RopeConnectPoint*> RopeConnectPoint::rope_connect_vector;

RopeConnectPoint::RopeConnectPoint(const Transform& transform):
	Geometry(transform)
{
	//プレイヤーのもつロープの接続ポイントリストに入れる
	rope_connect_vector.push_back(this);

	hook_ui = COPY_RESOURCE("hook_ui", CImage);
	hook_ui.SetSize(64, 64);
	hook_ui.SetCenter(32, 32);
}

void RopeConnectPoint::Update()
{
}

void RopeConnectPoint::Render()
{
	Geometry::Render();

	RopeUI();
}

void RopeConnectPoint::RopeUI()
{
	CVector3D view_pos = CameraManager::GetInstance()->GetPos();
	CVector3D view_rot = CameraManager::GetInstance()->GetRot();
	CVector3D target_pos = transform.position;
	if (Utility::IsViewInside(view_pos,view_rot, target_pos, DtoR(15), 16.0f)) {


		CVector2D image_pos;

		static float ang = DtoR(90.0f);

		ang = max(0.0f, ang - 0.1f);
		if (ang <= 0.0) {

			ang = DtoR(90.0f);

		}

		//スクリーン画像に直す
		Utility::WorldToScreen(&image_pos, transform.position);

		hook_ui.SetPos(image_pos);
		hook_ui.SetColor(1, 1, 1, sin(ang));

		hook_ui.Draw();
	}
}

bool RopeConnectPoint::InsideCamera(float ang,float length)
{
	//もしカメラの範囲にはいっているのなら
	if (CameraManager::GetInstance()->InsideCamera(transform.position,ang, length)) {

		return true;

	}

	return false;
}

