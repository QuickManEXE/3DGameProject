#pragma once
#include"ItemBase.h"


//基本アイテムオブジェクトクラス
class HeartItem : public ItemBase {
private:
	float recover_hp_num;

public:
	HeartItem(int _item_id, const Transform& transform);
	virtual void PickUpFunc(Player* _player);
};