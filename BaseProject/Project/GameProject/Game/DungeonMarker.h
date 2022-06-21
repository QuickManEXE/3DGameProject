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
	
	//タイルのデータ格納配列
	typedef std::vector<std::vector<int>> TileData;
	
	//部屋の入口のデータ
	struct EntranceData {
		int parent_room_num;//親の部屋の番号
		int id;//判別のためのid番号
		CVector3D position;//座標
		DirectionType dir;//扉の向き
		EntranceData(int _parent_room_num, const CVector3D& _position, DirectionType _dir) {
			parent_room_num = _parent_room_num;
			position = _position;
			dir = _dir;
		}
	};
	typedef std::vector<EntranceData> EntranceDatas;

	//部屋の大きさ
	typedef CRect RoomRect;
	//全部屋のデータ格納
	typedef std::vector<RoomRect> RoomData;
	//部屋の種類格納
	typedef std::vector<RoomRect> RoomIndex;
	static RoomIndex m_room_index;
	struct RoomData2 {
		RoomRect room_rect;//部屋の位置のデータ
		EntranceDatas entrances;//部屋に隣接している入口のデータ
	};
	typedef std::vector<RoomData2> RoomDatas2;
	
	//ダンジョンの総合データ
	struct DungeonData {
		TileData m_tile;//地形のデータ
		RoomData m_room;//部屋の情報
		RoomDatas2 m_rooms2;
		EntranceDatas m_entrances;//入口の情報
		DungeonData() {
			m_room.reserve(100);//あらかじめ100個容量を予約しておく
			m_rooms2.reserve(100);
			m_entrances.reserve(100);
		}
	};

public:
	//ダンジョンの生成
	static void CreateDungeon(DungeonData* _data,const CVector2D& _dungeon_size, const CVector2D& _first_room_size,const CVector2D& _first_room_pos, int _max_room_num);

	//条件に沿ったポイントを返します
	static bool GetRandomDungeonPos(DungeonData* _data,CVector3D* _p_pos, TileType _tile_type);

	//四角のマスの数を返します
	static int GetRectGridNum(const CRect& _rect);

	static std::vector<CVector3D> GetRoomPosition(const CRect& _rect);
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
	//指定された範囲でTileTypeの一致する座標の取得
	static std::vector<CVector3D> GetSurroundPos(const TileData& _t_data, const RoomRect& _r_data, TileType _tile_type);
	
	//部屋と入口のデータを関連づける
	static void ConnectEntraceToRoom(DungeonData* _data);

	//ダンジョンの部屋の確認
	static void DrawRoomIndex();

	//ダンジョンの構造の確認
	static void DrawDungeon(DungeonData* data);
};