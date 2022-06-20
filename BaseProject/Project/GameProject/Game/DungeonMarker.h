#pragma once
#include"../Base/Base.h"

//�_���W���������N���X
class DungeonMarker {
public:
	//�_���W�����̃^�C���̎��
	enum class TileType {
		outside_wall_id,//�O��
		inside_wall_id,//����
		room_id,//����
		entrance_id,//����
		way_id,//�ʘH
	};
	//�_���W���������ɕK�v�ȕ��p�̗�
	enum class DirectionType {
		direction_north,//�k
		direction_south,//��
		direction_west,//��
		direction_east,//��
		direction_count,//���p����
	};
	
	//�^�C���̃f�[�^�i�[
	typedef std::vector<std::vector<int>> TileData;
	//�����̑傫��
	typedef CRect RoomRect;
	//�S�����̃f�[�^�i�[
	typedef std::vector<RoomRect> RoomData;
	//�����̎�ފi�[
	typedef std::vector<RoomRect> RoomIndex;
	static RoomIndex m_room_index;
	//�����̓����̃f�[�^
	struct EntranceData {
		int parent_room_num;//�e�̕����̔ԍ�
		CVector3D position;//���W
		DirectionType dir;//���̌���
		EntranceData(int _parent_room_num, const CVector3D& _position, DirectionType _dir) {
			parent_room_num = _parent_room_num;
			position = _position;
			dir = _dir;
		}
	};
	typedef std::vector<EntranceData> EntranceDatas;
	
	//�_���W�����̑����f�[�^
	struct DungeonData {
		TileData m_tile;//�n�`�̃f�[�^
		RoomData m_room;//�����̏��
		EntranceDatas m_entrances;//�����̏��
		DungeonData() {
			m_room.reserve(100);//���炩����100�e�ʂ�\�񂵂Ă���
			m_entrances.reserve(100);
		}
	};

public:
	//�_���W�����̐���
	static void CreateDungeon(DungeonData* _data,const CVector2D& _dungeon_size, const CVector2D& _first_room_size,const CVector2D& _first_room_pos, int _max_room_num);

	//�����ɉ������|�C���g��Ԃ��܂�
	static bool GetRandomDungeonPos(DungeonData* _data,CVector3D* _p_pos, TileType _tile_type);

	//�l�p�̃}�X�̐���Ԃ��܂�
	static int GetRectGridNum(const CRect& _rect);

	static std::vector<CVector3D> GetRoomPosition(const CRect& _rect);
private:
	//�_���W�����̏�����
	static void DungeonInit(DungeonData* data, int width, int height, TileType init_tile);

	//
	// �_���W���������֐�
	//
	
	static void CreateFirstRoom(DungeonData* _data, const CVector2D& _first_room_size,const CVector2D& _first_room_pos);

	static void CreateNextRoom(DungeonData* _data, int _max_room_num);

	static void EraseTile(TileData* _t_data, const RoomRect& _r_data,TileType _tile_type);

	//�w�肵���l�p�̊O���𖄂߂�
	static void EraseTileAround(TileData* _t_data, const RoomRect& _r_data, TileType _tile_type);

	static bool IsCreateRoom(TileData* _t_data, const RoomRect& _r_data);

	
	//�_���W�����̕����̊m�F
	static void DrawRoomIndex();

	//�_���W�����̍\���̊m�F
	static void DrawDungeon(DungeonData* data);
};