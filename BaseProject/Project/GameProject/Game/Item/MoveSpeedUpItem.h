#pragma once
#include"ItemBase.h"


//基本アイテムオブジェクトクラス
class MoveSpeedUpItem : public ItemBase {
public:
	MoveSpeedUpItem(int _item_id, const Transform& transform);
	virtual void PickUpFunc(Player* _player);
};