#pragma once
#include"Geometry.h"



//ロープの基本クラス
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