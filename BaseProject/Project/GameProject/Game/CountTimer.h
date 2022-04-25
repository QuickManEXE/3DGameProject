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
	//�J�E���g�J�n name:���O set_time:�I������ �@ 
	*/
	bool SetTimer(const std::string,float _set_time);
	float GetTimer(const std::string);
};