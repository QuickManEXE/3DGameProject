#pragma once
#include"../Base/Base.h"

//ダンジョン生成クラス
class DungeonMarker {
public:
	//ダンジョンのタイルの種類
	enum class TileType {
		outside_wall_id,//外壁
		inside_wall_id,//内壁
		room_id,//部屋
		entrance_id,//入口
		way_id,//通路
	};
	//ダンジョン生成に必要な方角の列挙
	enum class DirectionType {
		direction_north,//北
		direction_south,//南
		direction_west,//西
		direction_east,//東
		direction_count,//方角総数
	};
	
	//タイルのデータ格納
	typedef std::vector<std::vector<int>> TileData;
	//部屋の大きさ
	typedef CRect RoomRect;
	//全部屋のデータ格納
	typedef std::vector<RoomRect> RoomData;
	//部屋の種類格納
	typedef std::vector<RoomRect> RoomIndex;
	static RoomIndex m_room_index;
	
	//ダンジョンの総合データ
	struct DungeonData {
		TileData m_tile;//地形のデータ
		RoomData m_room;//部屋の情報
		DungeonData() {
			m_room.reserve(100);//あらかじめ100個容量を予約しておく
		}
	};

public:
	//ダンジョンの生成
	static void CreateDungeon(DungeonData* _data,const CVector2D& _dungeon_size, const CVector2D& _first_room_size,const CVector2D& _first_room_pos, int _max_room_num);

	//条件に沿ったポイントを返します
	static bool GetRandomDungeonPos(DungeonData* _data,CVector3D* _p_pos, TileType _tile_type);

private:
	//ダンジョンの初期化
	static void DungeonInit(DungeonData* data, int width, int height, TileType init_tile);

	//
	// ダンジョン生成関数
	//
	
	static void CreateFirstRoom(DungeonData* _data, const CVector2D& _first_room_size,const CVector2D& _first_room_pos);

	static void CreateNextRoom(DungeonData* _data, int _max_room_num);

	static void EraseTile(TileData* _t_data, const RoomRect& _r_data,TileType _tile_type);

	//指定した四角の外周を埋める
	static void EraseTileAround(TileData* _t_data, const RoomRect& _r_data, TileType _tile_type);

	static bool IsCreateRoom(TileData* _t_data, const RoomRect& _r_data);

	
	//ダンジョンの部屋の確認
	static void DrawRoomIndex();

	//ダンジョンの構造の確認
	static void DrawDungeon(DungeonData* data);
};