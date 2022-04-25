#pragma once
#include"../stdafx.h"
#include<map>
#include<iostream>

class CountTimer {
private:
	std::map<std::string, float> m_timer_map;
public:
	CountTimer();
	~CountTimer();
	/*
	//カウント開始 name:名前 set_time:終了時間 　 
	*/
	bool SetTimer(const std::string,float _set_time);
	float GetTimer(const std::string);
};