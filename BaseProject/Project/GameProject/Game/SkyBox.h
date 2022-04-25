#pragma once
#include"../Base/Base.h"

class SkyBox : public Base {
private:
	RenderTask m_Render;

	CModel* m_skybox;
public:
	SkyBox();
	void Render();
};
