#include"Map.h"
#include"Player/Player.h"
#include"Geometry.h"


Map::Map() : Base(UpdatePriority::eUp_Field, "Map")
{
	DungeonMarker::CreateDungeon(&m_dungeon_data, CVector2D(MAP_WIDTH, MAP_HEIGHT), CVector2D(4, 4),CVector2D(MAP_WIDTH/2,MAP_HEIGHT/2), 10);

	m_render.RegistRender(this, (RenderFunc)&Map::Render, RenderPriority::eRd_Field, "Map");

	m_collide.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Map::CollisionCheck, CollisionPriority::eCol_Field, "Map");

	m_wall = COPY_RESOURCE("B222", CModelObj);

	m_ground = COPY_RESOURCE("DPlate", CModelObj);

	m_ground.SetScale(MAP_WIDTH * TILE_SIZE, MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE);
}

void Map::Render()
{
	m_ground.SetPos(CVector3D(MAP_WIDTH /2 * TILE_SIZE, -TILE_SIZE/2, MAP_HEIGHT /2 * TILE_SIZE));
	//m_ground.SetScale(CVector3D::one*50.0f);
	m_ground.Render();

	for (int h = 0; h < m_dungeon_data.m_tile.size(); h++) {

		for (int w = 0; w < m_dungeon_data.m_tile[0].size(); w++) {

			int tip = m_dungeon_data.m_tile[h][w];

			switch (tip)
			{
			case (int)DungeonMarker::TileType::outside_wall_id:
				m_wall.SetPos(CVector3D(w * TILE_SIZE, 0, h * TILE_SIZE));
				m_wall.SetScale(CVector3D(TILE_SIZE / 2.0f, 10 ,TILE_SIZE / 2.0f));
				m_wall.Render();
			default:
				break;
			}

		}

	}
}

void Map::CollisionCheck(CollisionTask* _task)
{
	//もしプレイヤーなら当たり判定する
	if (_task->GetPriority() != CollisionPriority::eCol_Player) return;

	if (Player* p = dynamic_cast<Player*> (_task->GetTask())) {

		//このキャラクターとオブジェクトとの当たり判定
		{
			//衝突データ格納用
			CCollTriangle out[256];
			//押し戻し量
			CVector3D v(0, 0, 0);

			//カプセルとモデルとの衝突
			int cnt = m_ground.CollisionCupsel(out, p->m_Capsule.m_start,
				p->m_Capsule.m_end, p->m_Capsule.m_rad, 250);

			//接触した面の数繰り返す
			for (int i = 0; i < cnt; i++) {

				//接触した面が斜めより上を向いていたら地面
				if (out[i].m_normal.y > 0.5f) {
					//重力落下速度を0に戻す
					p->m_Transform.m_pos_vec.y = 0;

					p->m_IsGround = true;
				}

				float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
				//接触した面の方向へ、めり込んだ分押し戻す
				CVector3D nv = out[i].m_normal * (p->m_Capsule.m_rad - out[i].m_dist);
				//最も大きな移動量を求める
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				//膝下までは乗り越える
				//膝上以上の壁のみ押し戻される
				if (max_y > p->m_Transform.position.y + 0.5f) {
					v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
					v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
				}

			}
			//押し戻す
			p->m_Transform.position += v;
		}

		//プレイヤーの位置
		CVector3D p_pos = p->m_Transform.position;
		//配列のどの位置にいるかXとZの大きさを求める
		int x = max(0, min(MAP_WIDTH, (int)p_pos.x / TILE_SIZE));

		int z = max(0, min(MAP_HEIGHT, (int)p_pos.z / TILE_SIZE));

		//プレイヤーの地点と周り8方向のマスと当たり判定する
		CVector2D index[9] = {
			CVector2D(-1,-1),CVector2D(0,-1),CVector2D(1,-1),
			CVector2D(-1,0),  CVector2D(0,0),CVector2D(1,0),
			CVector2D(-1,1),CVector2D(0,1),CVector2D(1,1)
		};

		//周り9回
		for (int i = 0; i < 9; i++) {

			int ix = x + index[i].x;

			int iz = z + index[i].y;

			//配列の外を指す場合は判定しない
			if (ix<0 || iz<0 || ix>=MAP_WIDTH || iz>=MAP_HEIGHT) continue;
			if (m_dungeon_data.m_tile[iz][ix] != (int)DungeonMarker::TileType::outside_wall_id)continue;


			//当たり判定用のモデルの位置を調整
			m_wall.SetPos(CVector3D(ix * TILE_SIZE, 0, iz * TILE_SIZE));
			m_wall.UpdateMatrix();

			//このキャラクターとオブジェクトとの当たり判定
			{
				//衝突データ格納用
				CCollTriangle out[256];
				//押し戻し量
				CVector3D v(0, 0, 0);

				//カプセルとモデルとの衝突
				int cnt = m_wall.CollisionCupsel(out, p->m_Capsule.m_start,
					p->m_Capsule.m_end, p->m_Capsule.m_rad, 250);

				//接触した面の数繰り返す
				for (int i = 0; i < cnt; i++) {

					//接触した面が斜めより上を向いていたら地面
					if (out[i].m_normal.y > 0.5f) {
						//重力落下速度を0に戻す
						p->m_Transform.m_pos_vec.y = 0;

						p->m_IsGround = true;
					}

					float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
					//接触した面の方向へ、めり込んだ分押し戻す
					CVector3D nv = out[i].m_normal * (p->m_Capsule.m_rad - out[i].m_dist);
					//最も大きな移動量を求める
					v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
					//膝下までは乗り越える
					//膝上以上の壁のみ押し戻される
					if (max_y > p->m_Transform.position.y + 0.5f) {
						v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
						v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
					}

				}
				//押し戻す
				p->m_Transform.position += v;
			}

		}
	}

}







