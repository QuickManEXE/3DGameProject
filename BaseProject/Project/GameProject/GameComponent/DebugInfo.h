#pragma once
#include"../Base/Base.h"


class DebugInfo : public Base{
private:
	RenderTask m_Render;

	CFont m_debug_text;

	DebugInfo* m_instance;
public:

	DebugInfo* GetInstance();
	DebugInfo();
	void Update();
	void Render();
	void Draw(float x,float y,float r,float g,float b,const char* format, ...);
};



