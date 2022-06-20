#include"Map.h"
#include"Player/Player.h"
#include"Geometry.h"


Map::Map() : Base(UpdatePriority::eUp_Field, "Map")
{
	

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
	//�����v���C���[�Ȃ瓖���蔻�肷��
	//if (_task->GetPriority() != CollisionPriority::eCol_Player) return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		//���̃L�����N�^�[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
		{
			//�Փ˃f�[�^�i�[�p
			CCollTriangle out[256];
			//�����߂���
			CVector3D v(0, 0, 0);

			//�J�v�Z���ƃ��f���Ƃ̏Փ�
			int cnt = m_ground.CollisionCupsel(out, c->m_Capsule.m_start,
				c->m_Capsule.m_end, c->m_Capsule.m_rad, 250);

			//�ڐG�����ʂ̐��J��Ԃ�
			for (int i = 0; i < cnt; i++) {

				//�ڐG�����ʂ��΂߂���������Ă�����n��
				if (out[i].m_normal.y > 0.5f) {
					//�d�͗������x��0�ɖ߂�
					c->m_Transform.m_pos_vec.y = 0;

					c->m_IsGround = true;
				}

				float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
				//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
				CVector3D nv = out[i].m_normal * (c->m_Capsule.m_rad - out[i].m_dist);
				//�ł��傫�Ȉړ��ʂ����߂�
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				//�G���܂ł͏��z����
				//�G��ȏ�̕ǂ̂݉����߂����
				if (max_y > c->m_Transform.position.y + 0.5f) {
					v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
					v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
				}

			}
			//�����߂�
			c->m_Transform.position += v;
		}

		//�v���C���[�̈ʒu
		CVector3D p_pos = c->m_Transform.position;
		//�z��̂ǂ̈ʒu�ɂ��邩X��Z�̑傫�������߂�
		int x = max(0, min(MAP_WIDTH, (int)p_pos.x / TILE_SIZE));

		int z = max(0, min(MAP_HEIGHT, (int)p_pos.z / TILE_SIZE));

		//�v���C���[�̒n�_�Ǝ���8�����̃}�X�Ɠ����蔻�肷��
		CVector2D index[9] = {
			CVector2D(-1,-1),CVector2D(0,-1),CVector2D(1,-1),
			CVector2D(-1,0),  CVector2D(0,0),CVector2D(1,0),
			CVector2D(-1,1),CVector2D(0,1),CVector2D(1,1)
		};

		//����9��
		for (int i = 0; i < 9; i++) {

			int ix = x + index[i].x;

			int iz = z + index[i].y;

			//�z��̊O���w���ꍇ�͔��肵�Ȃ�
			if (ix<0 || iz<0 || ix>=MAP_WIDTH || iz>=MAP_HEIGHT) continue;
			if (m_dungeon_data.m_tile[iz][ix] != (int)DungeonMarker::TileType::outside_wall_id)continue;


			//�����蔻��p�̃��f���̈ʒu�𒲐�
			m_wall.SetPos(CVector3D(ix * TILE_SIZE, 0, iz * TILE_SIZE));
			m_wall.UpdateMatrix();

			//���̃L�����N�^�[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
			{
				//�Փ˃f�[�^�i�[�p
				CCollTriangle out[256];
				//�����߂���
				CVector3D v(0, 0, 0);

				//�J�v�Z���ƃ��f���Ƃ̏Փ�
				int cnt = m_wall.CollisionCupsel(out, c->m_Capsule.m_start,
					c->m_Capsule.m_end, c->m_Capsule.m_rad, 250);

				//�ڐG�����ʂ̐��J��Ԃ�
				for (int i = 0; i < cnt; i++) {

					//�ڐG�����ʂ��΂߂���������Ă�����n��
					if (out[i].m_normal.y > 0.5f) {
						//�d�͗������x��0�ɖ߂�
						c->m_Transform.m_pos_vec.y = 0;

						c->m_IsGround = true;
					}

					float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
					//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
					CVector3D nv = out[i].m_normal * (c->m_Capsule.m_rad - out[i].m_dist);
					//�ł��傫�Ȉړ��ʂ����߂�
					v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
					//�G���܂ł͏��z����
					//�G��ȏ�̕ǂ̂݉����߂����
					if (max_y > c->m_Transform.position.y + 0.5f) {
						v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
						v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
					}

				}
				//�����߂�
				c->m_Transform.position += v;
			}

		}
	}

}

void Map::CreateDungeon(const CVector2D& _dungeon_size, const CVector2D& _first_room_size, const CVector2D& _first_room_pos, int _max_room_num)
{
	DungeonMarker::CreateDungeon(&m_dungeon_data, _dungeon_size,  _first_room_size, _first_room_pos,_max_room_num);
}

DungeonMarker::TileType Map::GetTip(const CVector3D& pos, int* ix, int* iy)
{

	//����v�Z
	int x = pos.x / TILE_SIZE;
	//��̐���
	if (x < 0) x = 0;
	if (x > MAP_WIDTH - 1) x = MAP_WIDTH - 1;
	if (ix) *ix = x;
	//�s���v�Z
	int y = pos.z / TILE_SIZE;
	//�s�̐���
	if (y < 0) y = 0;
	if (y > MAP_HEIGHT - 1) y = MAP_HEIGHT - 1;
	if (iy) *iy = y;
	//�`�b�v�f�[�^��ԋp
	return DungeonMarker::TileType(m_dungeon_data.m_tile[y][x]);

}
