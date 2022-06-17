#include"../../StarterAsset.h"
#include"Player.h"
#include"../Enemy/Enemy.h"
#include"State/StayState.h"
#include"State/IdleState.h"
#include"State/JumpState.h"
#include"State/HangWallState.h"
#include"State/HangDropState.h"
#include"State/ClimeState.h"
#include"State//BoxingState.h"
#include"State/GodState.h"
#include"State/WireAimState.h"
#include"State/WireShotState.h"
#include"State/WireActionState.h"
#include"State/ArrowAimState.h"
#include"State/PlayerDamageState.h"
#include"State/PlayerDeathState.h"

Player* Player::mp_Instance;

const char* Player::move_speed = "move_speed";
const char* Player::jump_power = "jump_power";
const char* Player::height = "height";
const char* Player::arm_range = "arm_range";

Player* Player::GetInstance()
{
	if (!mp_Instance) {

		return nullptr;

	}
	else return mp_Instance;
}

Player::Player(const Transform& _transform, const char* _modelName) :
CharacterBase(UpdatePriority::eUp_Player,_modelName,RenderPriority::eRd_Player)
{
	mp_Instance = this;

	SetProtect(true);

	m_Model = COPY_RESOURCE(_modelName, CModelA3M);

	m_bow = COPY_RESOURCE("Bow", CModelObj);

	this->m_Transform = _transform;

	// 描画タスクに登録
	//m_Render.RegistRender(this, (RenderFunc)&Player::Render, RenderPriority::eRd_Player, "Player");

	// 衝突判定タスクに登録
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Player::CollisionCheck, CollisionPriority::eCol_Player, "Player");

	// 攻撃衝突判定タスクに登録
	m_AttackCol.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Player::AttackCollision, CollisionPriority::eCol_Player_Attack, "Player");
	m_AttackCol.SetPause(true);

	m_Model.SetPos(_transform.position);
	m_Model.SetRot(_transform.rotation);
	m_Model.SetScale(_transform.scale);

	m_Param.SetParam(move_speed, 30.0f);
	m_Param.SetParam(jump_power, 30.0f);
	m_Param.SetParam(height, 2.0f);
	
	m_Param.SetParam("arm_height", 2.0f);


	m_IsGround = false;

	m_IsHook = false;

	m_Capsule.SetRad(0.2f);
	
	m_OldPos = _transform.position;

	//ステートAIの初期化（プレイヤー）
	m_StateAI.state_vector.push_back(new StayState(this));
	m_StateAI.state_vector.push_back(new IdleState(this));
	m_StateAI.state_vector.push_back(new JumpState(this));
	m_StateAI.state_vector.push_back(new HangWallState(this));
	m_StateAI.state_vector.push_back(new HangDropState(this));
	m_StateAI.state_vector.push_back(new ClimeState(this));
	m_StateAI.state_vector.push_back(new WireAimState(this));
	m_StateAI.state_vector.push_back(new WireShotState(this));
	m_StateAI.state_vector.push_back(new WireActionState(this));
	m_StateAI.state_vector.push_back(new ArrowAimState(this));
	m_StateAI.state_vector.push_back(new PlayerDamageState(this));
	m_StateAI.state_vector.push_back(new PlayerDeathState(this));

	m_StateAI.stateMachine = new StateMachine<Player>;
	
	m_StateAI.ChangeState(PlayerState::StayState);

	m_HookNum = 3;

	m_HookCoolTime = 0;

	m_HookRecoverTime = 15.0f;

	m_HitPoint = 100;
}

void Player::Update()
{
	m_StateAI.StateUpdate();
	m_IsGround = false;
	m_IsJump = true;
	HookRecover();
	UpdateCollider();
	CSound::GetInstance()->UpdateListener();
}

void Player::Render()
{
	StandardRender();

	CMatrix bow_matrix = m_Model.GetFrameMatrix("LeftHand")
		* CMatrix::MRotation(0,DtoR(90),0)  
		*CMatrix::MScale(100,100,100);
	m_bow.Render(bow_matrix);

	UpdateCollider();

#ifdef _DEBUG

	DrawDebugInfo();

#endif // DEBUG

}

void Player::CollisionCheck(CollisionTask* task)
{
	m_StateAI.StateCollisionCheck(task);
}

CVector3D Player::GetCenterPoint()
{
	return m_Model.GetFrameMatrix(4).GetPosition();
}

void Player::StandardRender()
{
	//キャラクター表示
	m_Model.SetPos(m_Transform.position);
	m_Model.SetRot(CVector3D(0,m_Transform.rotation.y,0));
	//胸のボーンは前方向へ向く
	m_Model.BindFrameMatrix(5, CMatrix::MRotation(m_Transform.rotation));
	m_Model.SetScale(m_Transform.scale);
	m_Model.Render();
}

void Player::HookRope(const CVector3D& _pos)
{

	//Rope* new_rope = new Rope(pos, transform.position, 10.0f);
	//rope = new_rope;
	//state_AI.ChangeState(PlayerState::HangRopeState);
	m_StateAI.ChangeState(PlayerState::WireActionState);
}

void Player::CollisionObject(CollisionTask* _task)
{

	if (_task->GetPriority() != CollisionPriority::eCol_Field)return;


	if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (_task->GetTask())) {
		

		CVector3D d = b->m_Transform.position - m_Transform.position;
		if (d.Length() > b->GetModel().GetMax().Length())return;

		

		//このキャラクターとオブジェクトとの当たり判定
		{
			//衝突データ格納用
			CCollTriangle out[256];
			//押し戻し量
			CVector3D v(0, 0, 0);

			//カプセルとモデルとの衝突
			int cnt = b->GetModel().CollisionCupsel(out, m_Capsule.m_start,
				m_Capsule.m_end, m_Capsule.m_rad, 250);

			//接触した面の数繰り返す
			for (int i = 0; i < cnt; i++) {

				//接触した面が斜めより上を向いていたら地面
				if (out[i].m_normal.y > 0.5f) {
					//重力落下速度を0に戻す
					m_Transform.m_pos_vec.y = 0;

					m_IsGround = true;
				}

				float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
				//接触した面の方向へ、めり込んだ分押し戻す
				CVector3D nv = out[i].m_normal * (m_Capsule.m_rad - out[i].m_dist);
				//最も大きな移動量を求める
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				//膝下までは乗り越える
				//膝上以上の壁のみ押し戻される
				if (max_y > m_Transform.position.y + 0.5f) {
					v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
					v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
				}
			}
			//押し戻す
			m_Transform.position += v;
		}
		

	}
}

void Player::HangCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Field)return;

	if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (_task->GetTask())) {

		CVector3D d = b->m_Transform.position - m_Transform.position;
		if (d.Length() > b->GetModel().GetMax().Length())return;

		
		//このキャラクター前方の２つの線とオブジェクトとの当たり判定(壁のぼり用)

		//接触点と法線
		CVector3D cross1, normal1;
		//プレイヤー左側の線と判定
		if (b->GetModel().CollisionRay(&cross1, &normal1, m_LineCol[0].m_start, m_LineCol[0].m_end)) {

			if (normal1.y < 0.5f)return;

			CVector3D cross2, normal2;
			//プレイヤー右側の線と判定
			if (b->GetModel().CollisionRay(&cross2, &normal2, m_LineCol[1].m_start, m_LineCol[1].m_end)) {

				//移動ベクトルを消す
				m_Transform.m_pos_vec.y = 0;

				m_ClimePoint = (cross1 + cross2) / 2.0f;

				m_StateAI.ChangeState(PlayerState::HangWallState);

			}
		}

	}
}
/*
void Player::CollisionRope(CollisionTask* task)
{
	if (task->GetPriority()!=CollisionPriority::eCol_Rope)return;
	if (m_is_hang_rope)return;

	if (Rope* b = dynamic_cast<Rope*> (task->GetTask())) {

		//カプセルと円との当たり判定
		CVector3D lineS = b->lineS;
		CVector3D lineE = b->lineE;
		float radC = b->m_rad;


		if (CCollision::CollisionCapsule(lineS, lineE, radC, m_capsule.m_start, m_capsule.m_end, m_capsule.m_rad)) {

			printf("プレイヤーロープにあたってます\n");

		}

		/*
		CVector3D posS(transform.position + CVector3D(0, 1.5f, 0)+ CMatrix::MRotation(transform.rotation).GetFront() * 0.4f);
		float radS = m_capsule.m_rad;

		//投影用の線分
		CVector3D p = posS - lineS;
		//線分のベクトル
		CVector3D line_dir = lineE - lineS;
		//最近点までの長さ
		float d_len = CVector3D::Dot(p, line_dir.GetNormalize());

		//線分上での点との最近点
		if (d_len < 0)
			d_len = 0;
		if (d_len > line_dir.Length())
			d_len = line_dir.Length();

		CVector3D C(lineS + line_dir.GetNormalize() * d_len);

		//点と線分との距離
		float length = (posS - C).Length();


		//球とカプセルが衝突していれば
		if (length < radS + radC) {

			printf("プレイヤーロープにあたってます\n");

			//rope = b;

			//b->transform.m_vec.x -= transform.m_vec.Length() / 1000.0f;
			
			//m_stateAI.ChangeState(PlayerState::WireActionState);
		}
		
	}
}
*/
void Player::DrawDebugInfo()
{
#ifdef _DEBUG
	//からだを覆うカプセル
	//Utility::DrawCapsule(chara_collider.s, chara_collider.e,chara_collider.rad, CVector4D(1, 0, 1, 0.1f));

	for (const CLine& l : m_LineCol) {
		Utility::DrawLine(l.m_start, l.m_end, CVector4D(1, 0.5, 0, 1.0f));
	}

	Utility::DrawSphere(m_ClimePoint, 0.2, CVector4D(0.1, 0.5, 0.6, 0.5f));//登るポイント

	Utility::DrawLine(m_JumpLine.m_start, m_JumpLine.m_end, CColorRGBA(0, 1, 0, 1));
	
	//Utility::DebugLocalDir(transform.position + CVector3D(0, m_param.GetParam("height"), 0), transform.rotation);


#endif // _DEBUG
}

void Player::AttackCollision(CollisionTask* _task)
{

	if (Enemy* b = dynamic_cast<Enemy*> (_task->GetTask())) {

		CVector3D d = b->m_Transform.position - m_Transform.position;

		if (d.Length() <= 10.0f) {


			CVector3D sh = m_Model.GetFrameMatrix(46).GetPosition();
			CVector3D ar = m_Model.GetFrameMatrix(48).GetPosition();
			//カプセルとモデルとの衝突
			if (CCollision::CollisionCapsuleShpere(sh, ar, 0.2f, b->m_Transform.position + CVector3D(0, 0.9, 0), 0.2f)) {

				
				printf("攻撃あたってる\n");
				b->m_HitPoint -= 100;

				if (b->m_HitPoint <= 0) {

					b->m_StateAI.ChangeState(EnemyState::DeathState);

				}
				else {

					b->m_StateAI.ChangeState(EnemyState::DamageState);

				}

				
			}
		}
	}

}

void Player::UpdateCollider()
{
	//ボーンの更新
	m_Model.SetPos(m_Transform.position);
	//m_model_a3m.SetRot(transform.rotation);
	m_Model.SetScale(m_Transform.scale);
	m_Model.UpdateMatrix();
	

	{//体のカプセルコライダー
		CVector3D start_pos = m_Model.GetFrameMatrix("Spine").GetPosition();
		CVector3D end_pos = m_Transform.position + CVector3D(0,m_Capsule.m_rad,0);

		start_pos = end_pos + CVector3D(0, 1.0, 0);
		m_Capsule.Set(start_pos, end_pos);
	}

	{//壁のぼり用線コライダー(左右2つ)

		CVector3D rot = CVector3D(0, m_Transform.rotation.y, 0);
		float front_len = 0.5f;
		float side_len = 0.25f;

		CVector3D lineS = m_Model.GetFrameMatrix("Neck").GetPosition()
			+ CVector3D(0, m_Param.GetParam("arm_height"), 0)
			+ CMatrix::MRotation(rot).GetFront() * front_len
			+ CMatrix::MRotation(rot).GetLeft() * side_len;

		CVector3D lineE = m_Model.GetFrameMatrix("Neck").GetPosition()
			+ CMatrix::MRotation(rot).GetFront() * front_len
			+ CMatrix::MRotation(rot).GetLeft() * side_len;

		m_LineCol[0].Set(lineS, lineE);


		lineS = m_Model.GetFrameMatrix("Neck").GetPosition()
			+ CVector3D(0, m_Param.GetParam("arm_height"), 0)
			+ CMatrix::MRotation(rot).GetFront() * front_len
			- CMatrix::MRotation(rot).GetLeft() * side_len;

		lineE = m_Model.GetFrameMatrix("Neck").GetPosition()
			+ CMatrix::MRotation(rot).GetFront() * front_len
			- CMatrix::MRotation(rot).GetLeft() * side_len;

		m_LineCol[1].Set(lineS, lineE);
	}

	{//ジャンプ用線コライダー

		CVector3D rot = CVector3D(0, m_Transform.rotation.y, 0);
		float front_len = 0.5f;
		float height = 1.0f;

		CVector3D lineS = m_Transform.position
			+ CMatrix::MRotation(rot).GetFront() * front_len;

		CVector3D lineE = m_Transform.position
			+ CMatrix::MRotation(rot).GetFront() * front_len;

		lineS += CVector3D(0, height, 0);
		lineE += CVector3D(0, -height, 0);

		m_JumpLine.Set(lineS,lineE);

	}
}

void Player::CollisionCheckToField(CollisionTask* _task){

	if (_task->GetPriority() != CollisionPriority::eCol_Field)return;

	if (NonAnimObject* object = dynamic_cast<NonAnimObject*> (_task->GetTask())) {


		CVector3D d = object->m_Transform.position - m_Transform.position;
		if (d.Length() > object->GetModel().GetMax().Length())return;

		//このキャラクターとオブジェクトとの当たり判定
		{
			//衝突データ格納用
			CCollTriangle out[256];
			//押し戻し量
			CVector3D v(0, 0, 0);

			//カプセルとモデルとの衝突
			int cnt = object->GetModel().CollisionCupsel(out, m_Capsule.m_start,
				m_Capsule.m_end, m_Capsule.m_rad, 250);

			//接触した面の数繰り返す
			for (int i = 0; i < cnt; i++) {

				//接触した面が斜めより上を向いていたら地面
				if (out[i].m_normal.y > 0.5f) {
					//重力落下速度を0に戻す
					m_Transform.m_pos_vec.y = 0;

					m_IsGround = true;
				}

				float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
				//接触した面の方向へ、めり込んだ分押し戻す
				CVector3D nv = out[i].m_normal * (m_Capsule.m_rad - out[i].m_dist);
				//最も大きな移動量を求める
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				//膝下までは乗り越える
				//膝上以上の壁のみ押し戻される
				if (max_y > m_Transform.position.y + 0.5f) {
					v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
					v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
				}

			}
			//押し戻す
			m_Transform.position += v;
		}


	}

	m_OldPos = m_Transform.position;
}

void Player::UpdateCamera()
{
	//キャラクターの行列
	CMatrix charactor_matrix;
	//支点の行列
	CMatrix parent_matrix;
	//カメラの行列
	CMatrix cam_matrix;
	//キャラクターの行列
	charactor_matrix = CMatrix::MTranselate(m_Transform.position);
	//支点の行列
	parent_matrix = charactor_matrix * CMatrix::MTranselate(CVector3D(0, 1.15, 0)) * CMatrix::MRotation(m_Transform.rotation);
	//カメラの行列
	cam_matrix = parent_matrix * CMatrix::MTranselate(CVector3D(-0.5, 0.334, -1.8));

	CCamera::GetCurrent()->SetTranseRot(cam_matrix);


	if (m_StateAI.IsCurrentState(PlayerState::ArrowAimState))
		m_fov = max(DtoR(30), m_fov - 0.1f);
	else
		m_fov = min(DtoR(60), m_fov + 0.1f);
	CCamera::GetCurrent()->Perspective(m_fov, (float)CCamera::GetCurrent()->GetWhidth() / (float)CCamera::GetCurrent()->GetHeight(), 0.01, 10000.0);;

}

void Player::HookRecover()
{
	//フックの数が3より少ないなら
	if (m_HookNum < 3) {


		m_HookCoolTime += DELTA;

		if(m_HookCoolTime >= m_HookRecoverTime) {

			m_HookNum++;
			m_HookCoolTime = 0;
		}

	}

}

void Player::CollisionIsJump(CollisionTask* _task)
{
	//もしジャンプ用の線コライダーがフィールドに当たってないなら
	if (_task->GetPriority() != CollisionPriority::eCol_Field)return;

	if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (_task->GetTask())) {

		CVector3D d = b->m_Transform.position - m_Transform.position;
		if (d.Length() > b->GetModel().GetMax().Length())return;

		//接触点と法線
		CVector3D cross1, normal1;
		//プレイヤー左側の線と判定
		if (b->GetModel().CollisionRay(&cross1, &normal1, m_JumpLine.m_start, m_JumpLine.m_end)) {

			m_IsJump = false;

		}

	}
}

