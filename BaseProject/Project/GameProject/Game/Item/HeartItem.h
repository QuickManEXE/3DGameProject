#pragma once
#include"ItemBase.h"


//��{�A�C�e���I�u�W�F�N�g�N���X
class HeartItem : public ItemBase {
private:
	float recover_hp_num;

public:
	HeartItem(int _item_id, const Transform& transform);
	virtual void PickUpFunc(Player* _player);
};