#pragma once
#include"../Base/Base.h"

class GUI :public Base
{
private:
	CImage m_target_icon;

	CImage m_lookon_icon;

	CImage m_img2;
	CImage m_img3;
	CImage Nomber_img;

	CImage m_hook_icon;
	CImage m_arrow_bow_icon;
	bool is_hook;

	RenderTask m_Draw;

	CFont m_text;

	CImage m_gui;

	float m_now_hp;
public:
	static float m_time_ang;
	GUI();
	void Update();
	void Draw();
	void DrawCircle(const CVector2D& pos, const float size, const float ang, const CVector4D& color);
	void DrawDubugInfo();
	void DrawAimTarget();
	void DrawAimTarget2();
	void DrawHP();
	void DrawIcon();
};