#pragma once
#include"Geometry.h"



//���[�v�̊�{�N���X
class RopeConnectPoint : public Geometry {
public:

	static std::vector<RopeConnectPoint*> rope_connect_vector;

private:

	CImage hook_ui;

public:
	RopeConnectPoint(const Transform& transform);
	void Update();
	void Render();
	void RopeUI();
	bool InsideCamera(float ang, float length);
};