#pragma once
#include "stdafx.h"

namespace StarterAsset {

static const char* Cube = "Cube";
static const char* Sphere = "Sphere";
static const char* Cylinder = "Cylinder";
static const char* Torus = "Torus";
static const char* Cone = "Cone";
static const char* Quad = "Quad";
static const char* Plate = "Plate";
static const char* Square = "Square";

static const char* xbot = "xbot";
static const char* ybot = "ybot";

	//汎用的なモデルデータ読み込み
	static void LoadStarterAsset() {

		//ジオメトリ―（基本図形）
		ADD_RESOURCE(Cube, CModel::CreateModel("StarterAsset/Geometry/Cube.obj"));
		ADD_RESOURCE(Sphere, CModel::CreateModel("StarterAsset/Geometry/Sphere.obj"));
		ADD_RESOURCE(Cylinder, CModel::CreateModel("StarterAsset/Geometry/Cylinder.obj"));
		ADD_RESOURCE(Torus, CModel::CreateModel("StarterAsset/Geometry/Torus.obj"));
		ADD_RESOURCE(Cone, CModel::CreateModel("StarterAsset/Geometry/Cone.obj"));
		ADD_RESOURCE(Quad, CModel::CreateModel("StarterAsset/Geometry/Quad.obj"));
		ADD_RESOURCE(Plate, CModel::CreateModel("StarterAsset/Geometry/Plate.obj")); 
		ADD_RESOURCE(Square, CModel::CreateModel("StarterAsset/Geometry/Square.obj"));

		//基本モデル
		ADD_RESOURCE(xbot, CModel::CreateModel("StarterAsset/Model/xbot/xbot.a3m"));
		ADD_RESOURCE(ybot, CModel::CreateModel("StarterAsset/Model/ybot/ybot.a3m"));

	}
}