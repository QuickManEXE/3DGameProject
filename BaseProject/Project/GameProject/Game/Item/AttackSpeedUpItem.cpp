#include "AttackSpeedUpItem.h"
#include"../../SoundResource.h"

AttackSpeedUpItem::AttackSpeedUpItem(int _item_id, const Transform& transform) : ItemBase(_item_id,transform,"Arrow")
{
}

void AttackSpeedUpItem::PickUpFunc(Player* _player)
{
	//printf("攻撃速度UPアイテムGET！");
	int attack_item_num = _player->GetAttackSpeedItemNum() + 1;
	_player->SetMoveSpeedItemNum(attack_item_num);
	int num = _player->GetAttackSpeedItemNum();
	_player->UpdateAttackSpeed(num);
	SOUND(SoundResource::SE_GetItem.c_str())->Play3D(m_Transform.position, CVector3D::zero);
}


