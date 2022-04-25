#pragma once
#include"../../Base/Base.h"


class Treasure : public Base {
protected:

	CModelObj m_model;

	RenderTask m_render;

public:
	Treasure(const Transform& transform);
	virtual void Render();
};