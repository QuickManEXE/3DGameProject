#pragma once
#include"../Enemy/Enemy.h"


class EnemySquad : public Base {
public:
	typedef std::vector<Enemy*> Enemys; 
	Enemys m_enemys;//�����̃|�C���^�[
	bool is_destruction; //�S�ł������ǂ���
	int enemy_num;//�����̐�
	bool is_active;//�A�N�e�B�u���ǂ���
public:
	EnemySquad();
	virtual ~EnemySquad();
	void Update();

	//�G�𐶐�����
	void SpawnEnemy(const CVector3D& pos);
	
	//�����������c���Ă��邩�m�F����
	bool GetIsDestruction();

};