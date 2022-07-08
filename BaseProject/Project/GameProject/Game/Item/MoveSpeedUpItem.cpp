#include "MoveSpeedUpItem.h"
#include"../../SoundResource.h"


MoveSpeedUpItem::MoveSpeedUpItem(int _item_id, const Transform& transform) : ItemBase(_item_id, transform, "Item_Shoe")
{
}

void MoveSpeedUpItem::PickUpFunc(Player* _player)
{
	//printf("�ړ��X�s�[�hUp�̃A�C�e�����Ƃ�܂���");
	int move_item_num = _player->GetMoveSpeedItemNum() + 1;
	_player->SetMoveSpeedItemNum(move_item_num);
	int num = _player->GetMoveSpeedItemNum();
	_player->UpdateMoveSpeed(num);
	SOUND(SoundResource::SE_GetItem.c_str())->Play3D(m_Transform.position, CVector3D::zero);
}

