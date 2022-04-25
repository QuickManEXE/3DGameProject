#pragma once
#include <map>


template<typename T = int>
class VariableParam {
private:
	std::map<const char* ,T> m_value_map;
public:
	//�p�����[�^�[�̐ݒ�
	void SetParam(const char* key,T value) {
		m_value_map[key] = value;
	};
	//�L�[�̒l�̎擾
	T GetParam(const char* key)
	{
		return m_value_map[key];
	};
	//�L�[�̒l�̎擾
	void ChangeParam(const char* key,T delta) {
		T m = GetParam(key);
		m += delta;
		SetParam(key, m);
	};
};