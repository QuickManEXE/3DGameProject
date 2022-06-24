#pragma once
#include "stdafx.h"


namespace Resource {

	static void LoadResource() {
		

		//画像
		ADD_RESOURCE("TitleBack", CImage::CreateImage("UI/Title.png"));
		ADD_RESOURCE("TitleLogo", CImage::CreateImage("UI/TitleLogo.png"));

		ADD_RESOURCE("hook_ui", CImage::CreateImage("UI/hook_ui.png"));

		ADD_RESOURCE("TargetIcon", CImage::CreateImage("UI/target.png"));
		ADD_RESOURCE("LookonIcon", CImage::CreateImage("UI/lookon.png"));

		ADD_RESOURCE("hook_icon", CImage::CreateImage("UI/hook.png"));
		ADD_RESOURCE("arrow_bow_icon", CImage::CreateImage("UI/arrow_bow.png"));

		ADD_RESOURCE("GUI", CImage::CreateImage("UI/GUI.png"));

		ADD_RESOURCE("GameClearBack", CImage::CreateImage("UI/GAMECLEAR.png"));
		ADD_RESOURCE("GameOverBack", CImage::CreateImage("UI/GAMEOVER.png"));

		ADD_RESOURCE("Transition1", CImage::CreateImage("UI/pipo-tr001.png"));
		ADD_RESOURCE("Transition2", CImage::CreateImage("UI/pipo-tr002.png"));
		ADD_RESOURCE("Transition3", CImage::CreateImage("UI/pipo-tr003.png"));
		ADD_RESOURCE("Transition4", CImage::CreateImage("UI/pipo-tr004.png"));
		ADD_RESOURCE("Transition5", CImage::CreateImage("UI/pipo-tr005.png"));
		//スカイボックス
		ADD_RESOURCE("Sky", CModel::CreateModel("Field/Sky/Sky.obj"));
		//モデル
		ADD_RESOURCE("Archer", CModel::CreateModel("Charactor/Archer/erika_archer.a3m"));
		ADD_RESOURCE("Archer2", CModel::CreateModel("Charactor/Archer/Archer.a3m"));
		ADD_RESOURCE("OpeningPlayer", CModel::CreateModel("Charactor/Akai/OpeningPlayer.a3m"));
		ADD_RESOURCE("Golem", CModel::CreateModel("Charactor/Golem/golem.a3m"));
		ADD_RESOURCE("FireElemental", CModel::CreateModel("Charactor/fire_elemental/fire_elemental.a3m"));

		//エフェクト
		ADD_RESOURCE("Bomb", CModel::CreateModel("Effect/Bomb.obj"));
		ADD_RESOURCE("Blood", CModel::CreateModel("Effect/Blood.obj"));
		ADD_RESOURCE("Blood2", CModel::CreateModel("Effect/Blood2.obj"));
		ADD_RESOURCE("Dust", CModel::CreateModel("Effect/Dust.obj"));
		ADD_RESOURCE("CircleR", CModel::CreateModel("Effect/CircleR.obj"));
		ADD_RESOURCE("Smoke", CModel::CreateModel("Effect/Smoke.obj"));
		ADD_RESOURCE("Slash", CModel::CreateModel("Effect/Slash.obj"));
		ADD_RESOURCE("bulletA", CModel::CreateModel("Effect/bulletA.obj"));

		//道具
		ADD_RESOURCE("Bow", CModel::CreateModel("Bow/Bow.obj"));
		ADD_RESOURCE("Arrow", CModel::CreateModel("Bow/arrow.obj"));

		ADD_RESOURCE("Rope", CModel::CreateModel("Rope/rope.obj"));
		ADD_RESOURCE("Rope_Short", CModel::CreateModel("Rope/rope_short.obj"));

		ADD_RESOURCE("Rock2", CModel::CreateModel("Rock/Rock2.obj"));
		ADD_RESOURCE("EffectRock2", CModel::CreateModel("Rock/Rock2.obj"));
		ADD_RESOURCE("Rock5", CModel::CreateModel("Rock/Rock5.obj"));

		ADD_RESOURCE("TreasureMap", CModel::CreateModel("TreasureMap/TreasureMap.obj"));
		
		
		ADD_RESOURCE("Mountain", CModel::CreateModel("Field/Mountain/mountain2.obj", 100, 10, 100));
		ADD_RESOURCE("OpeningMountain", CModel::CreateModel("Field/Mountain/OpeningMountain.obj", 100, 10, 100));
		ADD_RESOURCE("Cave", CModel::CreateModel("Field/Mountain/Cave.obj", 100, 10, 100));

		//小道具
		ADD_RESOURCE("Portal", CModel::CreateModel("Prop/Portal.obj"));
		ADD_RESOURCE("Chest_Top", CModel::CreateModel("Prop/Chest_Top.obj"));//
		ADD_RESOURCE("Chest_Bottom", CModel::CreateModel("Prop/Chest_Bottom.obj"));//
		
		//アイテム
		ADD_RESOURCE("Item_Heart", CModel::CreateModel("Item/Heart.obj"));//
		//ADD_RESOURCE("Item_Bullet", CModel::CreateModel("Wepon/Bullet/Bullet.obj"));//
		ADD_RESOURCE("Item_Shoe", CModel::CreateModel("Item/Shoe.obj"));//

		ADD_RESOURCE("B222", CModel::CreateModel("Block/Block2_2_2.obj"));
		ADD_RESOURCE("B224", CModel::CreateModel("Block/Block2_2_4.obj"));
		ADD_RESOURCE("B424", CModel::CreateModel("Block/Block4_2_4.obj"));
		ADD_RESOURCE("B226", CModel::CreateModel("Block/Block2_2_6.obj"));
		ADD_RESOURCE("DPlate", CModel::CreateModel("Block/Plate.obj"));
	}
}