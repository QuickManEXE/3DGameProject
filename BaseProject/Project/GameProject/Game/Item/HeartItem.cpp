#include "HeartItem.h"

HeartItem::HeartItem(int _item_id, const Transform& transform) : ItemBase(_item_id,transform,"Item_Heart"),recover_hp_num(30.0f)
{
}

void HeartItem::PickUpFunc(Player* _player)
{
	//printf("�̗�UP�񕜂��Ƃ�܂����I");
	float hp = _player->GetHitPoint() + recover_hp_num;
	_player->SetHitPoint(hp);
}

