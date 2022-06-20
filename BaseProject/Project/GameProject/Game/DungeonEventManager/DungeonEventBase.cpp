#include "DungeonEventBase.h"
#include <stdio.h>

DungeonEventBase::DungeonEventBase(DungeonMarker::RoomRect _room) : m_room(_room),is_active(false),is_complete(false)
{
}

DungeonEventBase::DungeonEventBase() : is_active(false), is_complete(false)
{
}

void DungeonEventBase::Enter()
{
	printf("�C�x���g�J�n\n");
}

void DungeonEventBase::Execute()
{
}

void DungeonEventBase::Exit()
{
	printf("�C�x���g�I��\n");
}
