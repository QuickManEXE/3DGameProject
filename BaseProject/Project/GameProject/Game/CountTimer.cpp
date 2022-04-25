#include "CountTimer.h"

CountTimer::CountTimer()
{
}

CountTimer::~CountTimer()
{
	m_timer_map.clear();
}

bool CountTimer::SetTimer(const std::string _name,float _set_time)
{
	//カウントダウン
	//キーが存在しないならタイマーをセットする
	float delta_time = CFPS::GetDeltaTime();

	m_timer_map[_name] = (m_timer_map.count(_name)==0)? 
		_set_time - delta_time : m_timer_map[_name] - delta_time;

	if (m_timer_map[_name] <= 0) {
		printf("%sのカウント完了\n",_name.c_str());
		m_timer_map[_name] = _set_time;
		return true;
	}
	return false;
}

float CountTimer::GetTimer(const std::string _name)
{
	//キーのタイマーがセットしてあるなら
	if (m_timer_map.count(_name) != 0) {
		return m_timer_map[_name];
	}
	//ないなら0をかえす
	return 0;
}
