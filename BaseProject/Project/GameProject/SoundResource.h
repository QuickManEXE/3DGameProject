#pragma once
#include "stdafx.h"

using namespace std;

//サウンドを管理する
namespace SoundResource {

	static const string SE_ShotArrowStart = "SE_ShotArrowStart";
	static const string SE_ShotArrowEnd = "SE_ShotArrowEnd";
	static const string SE_ArrowHit = "SE_ArrowHit";
	static const string SE_SlamGround = "SE_SlamGround";
	static const string SE_MonsterCry1 = "SE_MonsterCry1";
	static const string SE_MonsterCry2 = "SE_MonsterCry2";
	static const string SE_MonsterCry3 = "SE_MonsterCry3";
	static const string SE_MonsterFootSteps = "SE_MonsterFootSteps";
	static const string SE_Punch1 = "SE_Punch1";
	static const string SE_EarthQuake = "SE_EarthQuake";
	static const string SE_OpenChest = "SE_OpenChest";
	static const string SE_GetItem = "SE_GetItem";

	static const string BGM_Title = "BGM_Title";
	static const string BGM_LostPlace = "BGM_LostPlace";
	static const string BGM_Field = "BGM_Prairie4";
	static const string BGM_BossBattle = "BGM_BossBattle";
	static const string BGM_Fanfare = "BGM_Fanfare";
	static const string BGM_GAMEOVER = "BGM_GAMEOVER";

	static void SoundLoad(const string sound_name, bool bo3D = false)
	{

		if (sound_name.find("SE_") != string::npos) {
			//もし先頭がSEならSEフォルダから読み込む
			SOUND(sound_name.c_str())->Load(("Sound/SE/" + sound_name + ".wav").c_str(), bo3D);
			//printf("%s読込完了\n", sound_name.c_str());
		}
		else if (sound_name.find("BGM_") != string::npos) {
			//もし先頭がBGMならBGMフォルダから読み込む
			SOUND(sound_name.c_str())->Load(("Sound/BGM/" + sound_name + ".wav").c_str(), bo3D);
			//printf("%s読込完了\n", sound_name.c_str());
		}
		else {
			//printf("%sは読み込めませんでした\n", sound_name.c_str());
		}

	};
	
	static void LoadResource() {
		
		//SE
		SoundLoad(SE_ShotArrowStart, true);
		SoundLoad(SE_ShotArrowEnd, true);
		SoundLoad(SE_ArrowHit, true);
		SoundLoad(SE_SlamGround, true);
		SoundLoad(SoundResource::SE_Punch1, true);
		SoundLoad(SoundResource::SE_MonsterCry1, true);
		SoundLoad(SoundResource::SE_MonsterCry2, true);
		SoundLoad(SoundResource::SE_MonsterCry3, true);
		SoundLoad(SoundResource::SE_OpenChest, true);
		SoundLoad(SoundResource::SE_GetItem, true);
		//SoundLoad(SoundResource::SE_MonsterFootSteps);

		SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Load("Sound/SE/SE_MonsterFootSteps.wav");
		//SoundLoad(SoundResource::SE_EarthQuake);
		
		SOUND(SoundResource::SE_EarthQuake.c_str())->Load("Sound/SE/SE_EarthQuake.wav");
		
		//BGM
		//SoundLoad(SoundResource::BGM_Title);
		//SoundLoad(SoundResource::BGM_LostPlace);
		//SoundLoad(SoundResource::BGM_BossBattle);
		//SoundLoad(SoundResource::BGM_Fanfare);
		SOUND("BGM_Title")->Load("Sound/BGM/BGM_Title.wav");
		SOUND("BGM_LostPlace")->Load("Sound/BGM/BGM_LostPlace.wav");
		SOUND("BGM_BossBattle")->Load("Sound/BGM/BGM_BossBattle.wav");
		SOUND("BGM_Fanfare")->Load("Sound/BGM/BGM_Fanfare.wav");
		SOUND("BGM_GAMEOVER")->Load("Sound/BGM/BGM_GAMEOVER.wav");
	}

};