#pragma once
#include"../Base/Base.h"
#include"../Singleton/Singleton.h"
#include"Map.h"

class GUI :public Base,public Singleton<GUI>
{
	friend class Singleton<GUI>;
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

	//ミニマップ用のダンジョンのデータ　プレイヤーの半径がわかるようになっていく
	DungeonMarker::DungeonData m_DungeonData_For_MiniMap;
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
	void InitMiniMap(DungeonMarker::DungeonData _DungeonData_For_MiniMap); //ミニマップ初期化
	void DrawMiniMap(DungeonMarker::DungeonData _DungeonData_For_MiniMap);
	void UpdateMiniMap(DungeonMarker::DungeonData _DungeonData_For_MiniMap);
	void DrawGameInfo();
};