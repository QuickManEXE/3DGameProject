#include"GameTestState.h"
#include"../../Player/Player.h"
#include"../../Geometry.h"
#include"../../SkyBox.h"
#include"../../Map.h"
#include"../../GUI.h"
#include"../../FireElemental/FireElemental.h"
#include"../../Object/Goal.h"
#include"../../Enemy/Enemy.h"
#include"../../Object/Entrance.h"

GameTestState::GameTestState(GameManager* owner) : State(owner)
{
}

void GameTestState::Enter()
{
	//�_���W�����J�n�̏���������
	//�ڕW�̊K�w�ƊJ�n�̊K�w��ݒ�
	owner->m_CurrentDungeonNum = 1;
	owner->m_ClearDungeonNum = 5;

	//�X�J�C�{�b�N�X�̐���
	new SkyBox();

	//�v���C���[�̐���
	new Player(Transform(CVector3D(0, 0, 0), CVector3D(0, DtoR(0), 0), CVector3D(0.01f, 0.01f, 0.01f)), "Archer2");
	//�v���C���[�̃X�e�[�g��ʏ��Ԃɂ���
	Player::GetInstance()->m_StateAI.ChangeState(PlayerState::IdleState);

	//
	//new Geometry(Transform(CVector3D::zero, CVector3D::zero, CVector3D(10,10,10)), StarterAsset::Plate);

	//�_���W�����C���X�^���X�̐���
	Map::Build();
	//�_���W�����f�[�^�̐���
	Map::Instance().CreateDungeon(CVector2D(MAP_WIDTH,MAP_HEIGHT),CVector2D(4,4),CVector2D(MAP_WIDTH/2,MAP_HEIGHT/2),10);

	//�v���C���[�������_���Ȉʒu�ɔz�u
	CVector3D player_pos;

	if (DungeonMarker::GetRandomDungeonRoomPos(&Map::Instance().GetDungeonData(), &player_pos, Map::Instance().GetDungeonData().start_room_num)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		CVector3D pos(player_pos.x * TILE_SIZE, 0, player_pos.z * TILE_SIZE);

		Player::GetInstance()->m_Transform.position = pos;
	};
	/*if (DungeonMarker::GetRandomDungeonPos(&Map::Instance().GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		CVector3D pos(player_pos.x * TILE_SIZE, 0, player_pos.z * TILE_SIZE);
		
		Player::GetInstance()->m_Transform.position = pos;
	};*/

	//UI�C���X�^���X�̐���
	GUI::Build();
	//�~�j�}�b�v�f�[�^�̏�����
	GUI::Instance().InitMiniMap(Map::Instance().GetDungeonData());

	//�S�[���̐���
	Goal::Build();

	if (DungeonMarker::GetRandomDungeonRoomPos(&Map::Instance().GetDungeonData(), &player_pos, Map::Instance().GetDungeonData().goal_room_num)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);

		Goal::Instance().m_Transform = Transform(player_pos * TILE_SIZE + CVector3D(0, -1, 0), CVector3D::zero, CVector3D::one);

	};
	/*if (DungeonMarker::GetRandomDungeonPos(&Map::Instance().GetDungeonData(), &player_pos, DungeonMarker::TileType::room_id)) {
		printf("%f %f %f", player_pos.x, player_pos.y, player_pos.z);
		
		Goal::Instance().m_Transform = Transform(player_pos * TILE_SIZE + CVector3D(0,-1,0), CVector3D::zero, CVector3D::one);

	};*/

	//���̏o��
	DungeonMarker::DungeonData d_data = Map::Instance().GetDungeonData();
	DungeonMarker::RoomDatas2 r_data = d_data.m_rooms2;
	DungeonMarker::EntranceDatas e_data = d_data.m_entrances;

	for (int i = 0; i < e_data.size();i++) {
		
		const auto& entrance = e_data[i];
		CVector3D pos = entrance.position;
		
		CVector3D rot(0,0,0);
		switch (entrance.dir)
		{
		case DungeonMarker::DirectionType::direction_east:
		case DungeonMarker::DirectionType::direction_west:
			rot.y = DtoR(90);
			break;
		case DungeonMarker::DirectionType::direction_north:
		case DungeonMarker::DirectionType::direction_south:
			break;
		default:
			break;
		}
		//rot.y += DtoR(90);
		
		//new Geometry(Transform(pos * TILE_SIZE,rot, CVector3D(1, 1, 1)), StarterAsset::Cylinder);
		Entrance* e =  new Entrance(entrance.parent_room_num,Transform(pos * TILE_SIZE, rot, CVector3D(1, 1, 1)));
		
	}

	//�_���W�����C�x���g�V�X�e���̃C���X�^���X�̐���
	DungeonEventManager::Build();
	//�C�x���g�f�[�^�̓o�^
	DungeonEventManager::Instance().SetEvent(Map::Instance().GetDungeonData(), TILE_SIZE);

}

void GameTestState::Execute()
{
	DungeonEventManager::Instance().Run();
}

void GameTestState::Exit()
{
	owner->Destory();

	DungeonEventManager::Instance().RemoveEvents();
}

void GameTestState::Render()
{
	DungeonEventManager::Instance().RenderALLAABB();
}

void GameTestState::CollisionCheck(CollisionTask* _task)
{

	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		CVector3D pos = c->m_Transform.position;

		DungeonEventManager::Instance().CheckCollision(pos + CVector3D(0, 2.0f, 0));
	}


}
