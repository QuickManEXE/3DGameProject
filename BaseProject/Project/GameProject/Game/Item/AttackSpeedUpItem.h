#pragma once
#include"ItemBase.h"


//基本アイテムオブジェクトクラス
class AttackSpeedUpItem : public ItemBase {
public:
	AttackSpeedUpItem(int _item_id, const Transform& transform);
	virtual void PickUpFunc(Player* _player);
};