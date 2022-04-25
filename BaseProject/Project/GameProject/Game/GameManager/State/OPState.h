#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"

class OPState : public State<GameManager>,public ICamera {
private:
	float m_alpha;

	CImage m_title_logo;

	float m_fov;

	int m_cam_pos_num;

	static const int m_cam_pos_num_max = 4;

	static CVector3D m_cam_pos_index[m_cam_pos_num_max];

	CVector3D m_now_cam_pos;

	CVector3D m_next_cam_pos;

	float t;

public:
	OPState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void OPUpdate();

	void CreateField();

	void UpdateCamera()override;

	void CameraAtPos(const CVector3D& pos);

	void Render()override;
};