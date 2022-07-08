#include "HeartItem.h"
#include"../../SoundResource.h"

HeartItem::HeartItem(int _item_id, const Transform& transform) : ItemBase(_item_id,transform,"Item_Heart"),recover_hp_num(30.0f)
{
}

void HeartItem::PickUpFunc(Player* _player)
{
	//printf("‘Ì—ÍUP‰ñ•œ‚ð‚Æ‚è‚Ü‚µ‚½I");
	float hp = _player->GetHitPoint() + recover_hp_num;
	_player->SetHitPoint(hp);
	SOUND(SoundResource::SE_GetItem.c_str())->Play3D(m_Transform.position, CVector3D::zero);
}

