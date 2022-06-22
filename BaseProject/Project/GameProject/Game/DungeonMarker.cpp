#include"DungeonMarker.h"


DungeonMarker::RoomIndex DungeonMarker::m_room_index
{
	{CVector2D(0,0),CVector2D(2,4)},
	{CVector2D(0,0),CVector2D(2,2)},
	{CVector2D(0,0),CVector2D(6,6)},
	{CVector2D(0,0),CVector2D(6,8)},
	{CVector2D(0,0),CVector2D(8,6)},
	{CVector2D(0,0),CVector2D(8,8)},
	{CVector2D(0,0),CVector2D(10,10)},
};


void DungeonMarker::DungeonInit(DungeonData* _data,int _width,int _height,TileType _init_tile)
{
	if (!_data)return;

	TileData* t_data = &(_data->m_tile);
	

	//高さの大きさだけ初期化
	t_data->resize(_width);

	for (int h = 0; h < t_data->size();h++) {
		//幅の大きさだけ初期化
		(*t_data)[h].resize(_height);

		for (int w = 0; w < (*t_data)[h].size(); w++) {
			//タイルの初期化
			(*t_data)[h][w] = (int)_init_tile;

		}
	}
	
}

void DungeonMarker::CreateDungeon(DungeonData* _data, const CVector2D& _dungeon_size, const CVector2D& _first_room_size, const CVector2D& _first_room_pos, int _max_room_num)
{
	DungeonData data;

	srand(unsigned(time(NULL)));
	//srand(1);

	if (!_data) return;

	DungeonInit(&data,_dungeon_size.x, _dungeon_size.y, TileType::outside_wall_id);

	DrawRoomIndex();

	CreateFirstRoom(&data,_first_room_size,_first_room_pos);

	CreateNextRoom(&data,_max_room_num);

	ConnectEntraceToRoom(&data);

	SetStartAndGoalRoom(data);

	printf("\nダンジョンの生成が終わりました\n");

	DrawDungeon(&data);

	(*_data) = data;
}

void DungeonMarker::CreateFirstRoom(DungeonData* _data, const CVector2D& _first_room_size, const CVector2D& _first_room_pos)
{
	printf("\n\n\n");

	TileData* t_data = &(_data->m_tile);
	RoomData* r_data = &(_data->m_room);
	RoomDatas2* r_data2 = &(_data->m_rooms2);
	CVector2D center_room_size = _first_room_size;

	//全体の大きさを確認
	int dungeon_height = t_data->size();
	int dungeon_width = (*t_data)[0].size();

	printf("[タイルマップの大きさ]　幅＝%d　高さ＝%d\n", dungeon_width, dungeon_height);

	//今回のダンジョン作成方法
	//フロア連結型

	//①ダンジョンの真ん中に部屋を作る

	int center_x = _first_room_pos.x;
	int center_y = _first_room_pos.y;

	printf("[センタールームの位置]　X＝%d　Y＝%d\n", center_x, center_y);

	//４*４の部屋を真ん中に作る
	RoomRect center_room(center_x - (int)center_room_size.x / 2, center_y-(int)center_room_size.x / 2,
		center_x +(int)center_room_size.x / 2, center_y + (int)center_room_size.x / 2);

	r_data->push_back(center_room);
	RoomData2 center_room_data;
	center_room_data.room_rect = center_room;
	(*r_data2).push_back(center_room_data);

	EraseTile(t_data, center_room, TileType::room_id);

	//EraseTileAround(t_data, center_room, TileType::inside_wall_id);

}

void DungeonMarker::CreateNextRoom(DungeonData* data,int _max_room_num)
{
	//②ルームデータの中からランダムに部屋か通路を生成可能な面(branch_point)を見つける

	TileData* t_data = &(data->m_tile);
	RoomDatas2* rooms = &(data->m_rooms2);
	RoomData* room_data = &(data->m_room);
	EntranceDatas* e_data = &(data->m_entrances);
	int max_room_num = _max_room_num;
	
	for (const RoomData2& room_data2 : (*rooms)) {

		const RoomRect& room = room_data2.room_rect;

		for (int dir = (int)DirectionType::direction_north; dir < (int)DirectionType::direction_count; dir++) 
		{
			CVector2D branch_point;

			//作る
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

			printf("[branch_point]　X=%d　Y=%d\n", (int)branch_point.x, (int)branch_point.y);


			//③branch_pointの地点から部屋を生成する
			RoomRect next_room;
			//使う部屋をランダムで決めて
			int rand_id = rand() % m_room_index.size();
			//部屋を取り出す
			RoomRect use_room = m_room_index[rand_id];

			float point_dir = 0.0f;
			switch (dir)
			{
			case (int)DirectionType::direction_north:

				//北向きの場合
				//部屋がどれだけずれているか決める
				point_dir = rand() % (int)(use_room.m_width);

				next_room = RoomRect(branch_point.x - point_dir, branch_point.y - use_room.m_height,
					branch_point.x + use_room.m_width - point_dir, branch_point.y);

				break;
			case (int)DirectionType::direction_south:

				//南向きの場合
				//部屋がどれだけずれているか決める
				point_dir = rand() % (int)(use_room.m_width);

				next_room = RoomRect(branch_point.x - point_dir, branch_point.y + 1,
					branch_point.x + use_room.m_width - point_dir, branch_point.y + 1 + use_room.m_height);

				break;
			case (int)DirectionType::direction_west:

				//西向きの場合
				//部屋がどれだけずれているか決める
				point_dir = rand() % (int)(use_room.m_height);

				next_room = RoomRect(branch_point.x - use_room.m_width, branch_point.y - point_dir,
					branch_point.x, branch_point.y + use_room.m_height - point_dir);

				break;
			case (int)DirectionType::direction_east:
				//西向きの場合
				//部屋がどれだけずれているか決める
				point_dir = rand() % (int)(use_room.m_height);

				next_room = RoomRect(branch_point.x + 1, branch_point.y - point_dir,
					branch_point.x + 1 + use_room.m_width, branch_point.y + use_room.m_height - point_dir);

				break;
			default:
				break;
			}

			printf("[ズレ]＝%d　[ネクストルームの位置]　X＝%d　高さ＝%d\n",
				(int)point_dir, (int)next_room.m_left, (int)next_room.m_top);

			if (IsCreateRoom(t_data, next_room)) {

				
				room_data->push_back(next_room);

				RoomData2 next_room_data;
				next_room_data.room_rect = next_room;
				rooms->push_back(next_room_data);

				//EraseTileAround(t_data, next_room, TileType::inside_wall_id);

				(*t_data)[branch_point.y][branch_point.x] = (int)TileType::entrance_id;
				//エントランス情報の登録
				//e_data->push_back(EntranceData((int)room_data->size(), CVector3D(branch_point.x, 0, branch_point.y), DirectionType(dir)));
				e_data->push_back(EntranceData((int)rooms->size(), CVector3D(branch_point.x, 0, branch_point.y), DirectionType(dir)));

				EraseTile(t_data, next_room, TileType::room_id);

				DrawDungeon(data);

				CreateNextRoom(data,20);
			}
			else {

				printf("[部屋作れませんでした]\n");
				
			}
			if (room_data->size() >= max_room_num)break;
		}
		if (room_data->size() >= max_room_num)break;
	}

	printf("\n[これ以上は部屋を作れません]\n");
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

	//上のラインと下のライン
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

std::vector<CVector3D> DungeonMarker::GetSurroundPos(const TileData& _t_data, const RoomRect& _r_data, TileType _tile_type)
{
	std::vector<CVector3D> positions;

	TileData t_data = _t_data;
	RoomRect r_data = _r_data;

	for (int h = r_data.m_top - 1; h < r_data.m_bottom + 1; h++) {

		if (h < 0 || h >= t_data.size())return positions;

		for (int w = r_data.m_left - 1; w < r_data.m_right + 1; w++) {

			if (w < 0 || w >= t_data[0].size())return positions;

			if (t_data[h][w] == (int)_tile_type) {
				
				positions.push_back(CVector3D(w, 0, h));

			}

		}

	}

	return positions;
}

void DungeonMarker::ConnectEntraceToRoom(DungeonData* _data)
{
	if (!_data)return;

	DungeonData* d_data = _data;
	TileData t_data = d_data->m_tile;
	RoomDatas2 rooms = d_data->m_rooms2;

	for (RoomData2& room : rooms) {

		RoomRect rect = room.room_rect;
		RoomRect surround_rect = RoomRect(rect.m_left - 1, rect.m_top - 1, rect.m_right + 1, rect.m_bottom + 1);

		auto positions = GetSurroundPos(t_data, surround_rect,TileType::entrance_id);

		for (auto position : positions)
		{
			for (const EntranceData& entrance : (*d_data).m_entrances) {

				if (position == entrance.position) {

					room.entrances.push_back(entrance);

				}

			}
		}

	}


}

bool DungeonMarker::GetRandomDungeonPos(DungeonData* _data, CVector3D* _p_pos, TileType _tile_type)
{
	if (!_data)return false;
	if (!_p_pos)return false;

	TileData* t_data = &(_data->m_tile);

	//全体の大きさを確認
	int dungeon_height = t_data->size();
	int dungeon_width = (*t_data)[0].size();

	//今回の条件に合うものを格納する配列
	std::vector<CVector3D> vectors;

	for (int z = 0; z < dungeon_height;z++) {

		for (int x= 0; x < dungeon_width; x++) {

			auto id = (*t_data)[z][x];
			if (id == (int)_tile_type) {

				vectors.push_back(CVector3D(x,0,z));

			}
		}
	}

	//条件に合うものがなかったなら
	if (vectors.size() <= 0) return false;

	int index = Utility::Rand(0, (int)vectors.size());
	CVector3D pos = vectors[index];
	(*_p_pos) = pos;

	return true;
}

bool DungeonMarker::GetRandomDungeonRoomPos(DungeonData* _data, CVector3D* _p_pos, int _room_num)
{
	if (!_p_pos)return false;
	if (!_data)return false;

	DungeonMarker::RoomDatas2 rooms = _data->m_rooms2;
	int index = max(0,min(rooms.size(),_room_num));
	auto room = rooms[index];

	auto positions = GetRoomPosition(room.room_rect);

	if (positions.size() <= 0)return false;

	index = Utility::Rand(0, (int)positions.size());
	
	CVector3D pos = positions[index];

	(*_p_pos) = pos;

	return true;
}

int DungeonMarker::GetRectGridNum(const CRect& _rect)
{
	CRect rect = _rect;

	int grid_num= rect.m_width* rect.m_height;

	return grid_num;
}

std::vector<CVector3D> DungeonMarker::GetRoomPosition(const CRect& _rect)
{
	CRect rect = _rect;

	std::vector<CVector3D> positions;

	for (int z = rect.m_top; z < rect.m_bottom; z++) {

		for (int x = rect.m_left; x < rect.m_right; x++) {

			positions.push_back(CVector3D(x, 0, z));

		}
	}
	
	return positions;
}

void DungeonMarker::SetStartAndGoalRoom(DungeonData& _data)
{

	DungeonData& d_data = _data;

	RoomDatas2 rooms = d_data.m_rooms2;

	int max = rooms.size();

	int start_room_index = Utility::Rand(0, max);
	//スタートをきめる
	d_data.start_room_num = start_room_index;

	int goal_room_index;
	do {
		goal_room_index = Utility::Rand(0, max);
	} while (start_room_index == goal_room_index);

	//ゴールを決める
	d_data.goal_room_num = goal_room_index;

	
}

void DungeonMarker::DrawRoomIndex()
{
	printf("\n\n\n");
	printf("[今回使うフロアパーツ]\n");

	int num = 1;


	for (const RoomRect& room : m_room_index) {

		printf("\n\n");
		printf("[%d]\n", num++);
		for (int i = 0; i < room.m_height; i++) {

			printf("\n");
			for (int j = 0; j < room.m_width; j++) {

				printf("□");

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
				str = "■";
				break;
			case (int)TileType::inside_wall_id:
				str = "〇";
				break;
			case (int)TileType::room_id:
				str = "　";
				break;
			case (int)TileType::entrance_id:
				str = "＊";
				break;
			case (int)TileType::way_id:
				str = "・";
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
