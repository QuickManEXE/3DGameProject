#pragma once
#include <map>


template<typename T = int>
class VariableParam {
private:
	std::map<const char* ,T> m_value_map;
public:
	//パラメーターの設定
	void SetParam(const char* key,T value) {
		m_value_map[key] = value;
	};
	//キーの値の取得
	T GetParam(const char* key)
	{
		return m_value_map[key];
	};
	//キーの値の取得
	void ChangeParam(const char* key,T delta) {
		T m = GetParam(key);
		m += delta;
		SetParam(key, m);
	};
};