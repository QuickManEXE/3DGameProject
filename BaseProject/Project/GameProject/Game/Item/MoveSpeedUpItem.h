#pragma once
#include"ItemBase.h"


//��{�A�C�e���I�u�W�F�N�g�N���X
class MoveSpeedUpItem : public ItemBase {
public:
	MoveSpeedUpItem(int _item_id, const Transform& transform);
	virtual void PickUpFunc(Player* _player);
};