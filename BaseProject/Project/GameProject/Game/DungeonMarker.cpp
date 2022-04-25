#include"DungeonMarker.h"


DungeonMarker::RoomIndex DungeonMarker::m_room_index
{
	{CVector2D(0,0),CVector2D(2,4)},
	{CVector2D(0,0),CVector2D(2,2)},
	{CVector2D(0,0),CVector2D(4,4)},
	{CVector2D(0,0),CVector2D(2,6)},
};


void DungeonMarker::DungeonInit(DungeonData* _data,int _width,int _height,TileType _init_tile)
{
	if (!_data)return;

	TileData* t_data = &(_data->m_tile);

	//�����̑傫������������
	t_data->resize(_width);

	for (int h = 0; h < t_data->size();h++) {
		//���̑傫������������
		(*t_data)[h].resize(_height);

		for (int w = 0; w < (*t_data)[h].size(); w++) {
			//�^�C���̏�����
			(*t_data)[h][w] = (int)_init_tile;

		}
	}
	
}

void DungeonMarker::CreateDungeon(DungeonData* _data, const CVector2D& _dungeon_size, const CVector2D& _first_room_size, const CVector2D& _first_room_pos, int _max_room_num)
{
	srand(time(NULL));
	srand(1);

	if (!_data) return;

	DungeonInit(_data,_dungeon_size.x, _dungeon_size.y, TileType::outside_wall_id);

	DrawRoomIndex();

	CreateFirstRoom(_data,_first_room_size,_first_room_pos);

	CreateNextRoom(_data,_max_room_num);

	printf("\n�_���W�����̐������I���܂���\n");

	DrawDungeon(_data);
}

void DungeonMarker::CreateFirstRoom(DungeonData* _data, const CVector2D& _first_room_size, const CVector2D& _first_room_pos)
{
	printf("\n\n\n");

	TileData* t_data = &(_data->m_tile);
	RoomData* r_data = &(_data->m_room);
	CVector2D center_room_size = _first_room_size;

	//�S�̂̑傫�����m�F
	int dungeon_height = t_data->size();
	int dungeon_width = (*t_data)[0].size();

	printf("[�^�C���}�b�v�̑傫��]�@����%d�@������%d\n", dungeon_width, dungeon_height);

	//����̃_���W�����쐬���@
	//�t���A�A���^

	//�@�_���W�����̐^�񒆂ɕ��������

	int center_x = _first_room_pos.x;
	int center_y = _first_room_pos.y;

	printf("[�Z���^�[���[���̈ʒu]�@X��%d�@Y��%d\n", center_x, center_y);

	//�S*�S�̕�����^�񒆂ɍ��
	RoomRect center_room(center_x - (int)center_room_size.x / 2, center_y-(int)center_room_size.x / 2,
		center_x +(int)center_room_size.x / 2, center_y + (int)center_room_size.x / 2);

	r_data->push_back(center_room);

	EraseTile(t_data, center_room, TileType::room_id);

	//EraseTileAround(t_data, center_room, TileType::inside_wall_id);

}

void DungeonMarker::CreateNextRoom(DungeonData* data,int _max_room_num)
{
	//�A���[���f�[�^�̒����烉���_���ɕ������ʘH�𐶐��\�Ȗ�(branch_point)��������

	TileData* t_data = &(data->m_tile);
	RoomData* room_data = &(data->m_room);
	int max_room_num = _max_room_num;
	
	for (const RoomRect& room : (*room_data)) {

		for (int dir = (int)DirectionType::direction_north; dir < (int)DirectionType::direction_count; dir++) 
		{
			CVector2D branch_point;

			//���
			switch (dir)
			{
			case (int)DirectionType::direction_north:
				branch_point.y = room.m_top - 1;

				branch_point.x = (room.m_left + rand() % (int)(room.m_right - room.m_left));
				break;
			case (int)DirectionType::direction_south:
				branch_point.y = room.m_bottom;

				branch_point.x = (room.m_left + rand() % (int)(room.m_right - room.m_left));
				break;
			case (int)DirectionType::direction_west:
				branch_point.y = (room.m_top + rand() % (int)(room.m_bottom - room.m_top));

				branch_point.x = room.m_left - 1;
				break;
			case (int)DirectionType::direction_east:
				branch_point.y = (room.m_top + rand() % (int)(room.m_bottom - room.m_top));

				branch_point.x = room.m_right;
				break;
			default:
				break;
			}

			printf("[branch_point]�@X=%d�@Y=%d\n", (int)branch_point.x, (int)branch_point.y);


			//�Bbranch_point�̒n�_���畔���𐶐�����
			RoomRect next_room;
			//�g�������������_���Ō��߂�
			int rand_id = rand() % m_room_index.size();
			//���������o��
			RoomRect use_room = m_room_index[rand_id];

			float point_dir = 0.0f;
			switch (dir)
			{
			case (int)DirectionType::direction_north:

				//�k�����̏ꍇ
				//�������ǂꂾ������Ă��邩���߂�
				point_dir = rand() % (int)(use_room.m_width);

				next_room = RoomRect(branch_point.x - point_dir, branch_point.y - use_room.m_height,
					branch_point.x + use_room.m_width - point_dir, branch_point.y);

				break;
			case (int)DirectionType::direction_south:

				//������̏ꍇ
				//�������ǂꂾ������Ă��邩���߂�
				point_dir = rand() % (int)(use_room.m_width);

				next_room = RoomRect(branch_point.x - point_dir, branch_point.y + 1,
					branch_point.x + use_room.m_width - point_dir, branch_point.y + 1 + use_room.m_height);

				break;
			case (int)DirectionType::direction_west:

				//�������̏ꍇ
				//�������ǂꂾ������Ă��邩���߂�
				point_dir = rand() % (int)(use_room.m_height);

				next_room = RoomRect(branch_point.x - use_room.m_width, branch_point.y - point_dir,
					branch_point.x, branch_point.y + use_room.m_height - point_dir);

				break;
			case (int)DirectionType::direction_east:
				//�������̏ꍇ
				//�������ǂꂾ������Ă��邩���߂�
				point_dir = rand() % (int)(use_room.m_height);

				next_room = RoomRect(branch_point.x + 1, branch_point.y - point_dir,
					branch_point.x + 1 + use_room.m_width, branch_point.y + use_room.m_height - point_dir);

				break;
			default:
				break;
			}

			printf("[�Y��]��%d�@[�l�N�X�g���[���̈ʒu]�@X��%d�@������%d\n",
				(int)point_dir, (int)next_room.m_left, (int)next_room.m_top);

			if (IsCreateRoom(t_data, next_room)) {

				room_data->push_back(next_room);

				//EraseTileAround(t_data, next_room, TileType::inside_wall_id);

				(*t_data)[branch_point.y][branch_point.x] = (int)TileType::entrance_id;

				EraseTile(t_data, next_room, TileType::room_id);

				DrawDungeon(data);

				CreateNextRoom(data,20);
			}
			else {

				printf("[�������܂���ł���]\n");
				
			}
			if (room_data->size() >= max_room_num)break;
		}
		if (room_data->size() >= max_room_num)break;
	}

	printf("\n[����ȏ�͕��������܂���]\n");
}

void DungeonMarker::EraseTile(TileData* _t_data, const RoomRect& _r_data,TileType _tile_type)
{
	TileData* t_data = _t_data;
	RoomRect room = _r_data;

	for (int h = room.m_top; h < room.m_bottom; h++)
	{
		for (int w = room.m_left; w < room.m_right; w++) {

			(*t_data)[h][w] = (int)_tile_type;
		}
	}
}

void DungeonMarker::EraseTileAround(TileData* _t_data, const RoomRect& _r_data, TileType _tile_type)
{
	TileData* t_data = _t_data;
	RoomRect room = _r_data;

	//��̃��C���Ɖ��̃��C��
	for (int w = room.m_left - 1; w < room.m_right + 1; w++) {

		int h = room.m_top - 1;
		(*t_data)[h][w] = (int)_tile_type;
		h = room.m_bottom;
		(*t_data)[h][w] = (int)_tile_type;
	}

	for (int h = room.m_top; h < room.m_bottom; h++) {

		int w = room.m_left-1;
		(*t_data)[h][w] = (int)_tile_type;
		w = room.m_right;
		(*t_data)[h][w] = (int)_tile_type;
	}
}

bool DungeonMarker::IsCreateRoom(TileData* _t_data, const RoomRect& _r_data)
{
	if (!_t_data)return false;

	TileData* t_data = _t_data;
	RoomRect r_data = _r_data;

	bool is_create = true;

	for (int h = r_data.m_top-1; h < r_data.m_bottom+1; h++) {

		if (h<0||h >= t_data->size())return false;
		for (int w = r_data.m_left-1; w < r_data.m_right+1; w++) {

			if (w<0||w >= (*t_data)[0].size())return false;
			if ((*t_data)[h][w] != (int)TileType::outside_wall_id&&(int)TileType::inside_wall_id) {
				is_create = false;
			}

		}

	}

	return is_create;
}

void DungeonMarker::DrawRoomIndex()
{
	printf("\n\n\n");
	printf("[����g���t���A�p�[�c]\n");

	int num = 1;


	for (const RoomRect& room : m_room_index) {

		printf("\n\n");
		printf("[%d]\n", num++);
		for (int i = 0; i < room.m_height; i++) {

			printf("\n");
			for (int j = 0; j < room.m_width; j++) {

				printf("��");

			}

		}

	}


}

void DungeonMarker::DrawDungeon(DungeonData* _data)
{
	printf("\n\n\n");

	TileData* t_data = &(_data->m_tile);

	for (auto vector : (*t_data)) {

		for (int i = 0; i < vector.size();i++) {

			auto id = vector[i];
			char* str="";
			switch (id)
			{
			case (int)TileType::outside_wall_id:
				str = "��";
				break;
			case (int)TileType::inside_wall_id:
				str = "�Z";
				break;
			case (int)TileType::room_id:
				str = "�@";
				break;
			case (int)TileType::entrance_id:
				str = "��";
				break;
			case (int)TileType::way_id:
				str = "�E";
				break;
			default:
				break;
			}
			if (i == 0)printf("\n");
			printf("%s", str);

		}
	}

	printf("\n\n\n");
}