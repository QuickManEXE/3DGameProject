#pragma once
#include"../Enemy/Enemy.h"


class EnemySquad : public Base {
public:
	typedef std::vector<Enemy*> Enemys; 
	Enemys m_enemys;//部隊のポインター
	bool is_destruction; //全滅したかどうか
	int enemy_num;//部隊の数
	bool is_active;//アクティブかどうか
public:
	EnemySquad();
	virtual ~EnemySquad();
	void Update();

	//敵を生成する
	void SpawnEnemy(const CVector3D& pos);
	
	//部隊が生き残っているか確認する
	bool GetIsDestruction();

};