#include "AttackSpeedUpItem.h"

AttackSpeedUpItem::AttackSpeedUpItem(int _item_id, const Transform& transform) : ItemBase(_item_id,transform,"Arrow")
{
}

void AttackSpeedUpItem::PickUpFunc(Player* _player)
{
	//printf("�U�����xUP�A�C�e��GET�I");
	int attack_item_num = _player->GetAttackSpeedItemNum() + 1;
	_player->SetMoveSpeedItemNum(attack_item_num);
	int num = _player->GetAttackSpeedItemNum();
	_player->UpdateAttackSpeed(num);
}


