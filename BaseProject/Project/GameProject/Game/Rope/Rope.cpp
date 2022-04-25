#include"Rope.h"


Rope::Rope(const Transform& transform, float length, Rope* rope)
	: Base(UpdatePriority::eUp_Field,"Rope")
{
	
	// 描画タスクに登録
	m_Render.RegistRender(this, (RenderFunc)&Rope::Render, RenderPriority::eRd_Field, "Rope");
	// 当たり判定タスクに登録
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Rope::CollisionCheck, CollisionPriority::eCol_Rope, "Rope");

	m_rad = 0.1f;

	m_model = COPY_RESOURCE("Rope_Short", CModelObj);

	this->m_parent_rope = rope;

	this->m_Transform = transform;

	if (m_parent_rope) {

		this->lineS = m_parent_rope->lineE;
		this->m_Transform.rotation = m_parent_rope->m_Transform.rotation*0.9f;
	}
	else {

		
		this->lineS = transform.position;

	}

	this->rope_length = length;

	m_top_matrix = CMatrix::MRotation(transform.rotation);

	lineE = lineS + -m_top_matrix.GetUp() * rope_length;
}

void Rope::Update()
{
	
	//角度を
	if (m_Transform.rotation.x > 0) {
		float s = sin(m_Transform.rotation.x);
		m_Transform.m_rot_vec.x -= DtoR(90.0) * s * DELTA;
		//printf("角度%f,ぱわー%f,比率%f,\n", RtoD(transform.rotation.x), RtoD(transform.m_rot_vec.x), s);
	}
	else if (m_Transform.rotation.x < 0) {
		float s = sin(m_Transform.rotation.x);
		m_Transform.m_rot_vec.x -= DtoR(90.0) * s * DELTA;
		//printf("角度%f,ぱわー%f,比率%f,\n", RtoD(transform.rotation.x), RtoD(transform.m_rot_vec.x), s);
	}

	if (m_Transform.rotation.z > 0) {
		float s = sin(m_Transform.rotation.z);
		m_Transform.m_rot_vec.z -= DtoR(90.0) * s * DELTA;
		//printf("角度%f,ぱわー%f,比率%f,\n", RtoD(transform.rotation.x), RtoD(transform.m_rot_vec.x), s);
	}
	else if (m_Transform.rotation.z < 0) {
		float s = sin(m_Transform.rotation.z);
		m_Transform.m_rot_vec.z -= DtoR(90.0) * s * DELTA;
		//printf("角度%f,ぱわー%f,比率%f,\n", RtoD(transform.rotation.x), RtoD(transform.m_rot_vec.x), s);
	}
	

	m_Transform.rotation += m_Transform.m_rot_vec * DELTA;
	//transform.m_rot_vec *= (1 - FRICTION);

	CMatrix m_top_matrix = CMatrix::MRotation(m_Transform.rotation);

	if (m_parent_rope) {

		this->lineS = m_parent_rope->lineE;

	}
	else {


		this->lineS = m_Transform.position;

	}

	lineE = lineS + -m_top_matrix.GetUp() * rope_length;

}

void Rope::Render()
{
	//Utility::DrawCapsule(lineS,lineE,m_rad,CVector4D(0, 0, 0.5, 0.5f));
	
	m_model.SetPos(lineS);
	m_model.SetRot(m_Transform.rotation+CVector3D(DtoR(0),0,DtoR(0)));
	m_model.SetScale(CVector3D(0.2f,0.2f,0.2f));
	m_model.Render();
	
}

void Rope::CollisionCheck(CollisionTask* _task)
{
}

WireRope::WireRope(Player* mp_p, Arrow* mp_a) : Base(UpdatePriority::eUp_Field, "WireRope")
{

	m_model = COPY_RESOURCE("Rope_Short", CModelObj);

	mp_player = mp_p;
	mp_arrow = mp_a;

	// 描画タスクに登録
	m_Render.RegistRender(this, (RenderFunc)&WireRope::Render, RenderPriority::eRd_Field, "WireRope");

	CVector3D start_pos = mp_player->GetModel()->GetFrameMatrix(50).GetPosition();
	CVector3D end_pos = mp_arrow->m_Transform.position;

	SetLine(start_pos, end_pos);
	
}

void WireRope::Update()
{
	CVector3D start_pos = mp_player->GetModel()->GetFrameMatrix(50).GetPosition();
	CVector3D end_pos = mp_arrow->m_Transform.position;

	SetLine(start_pos, end_pos);

	
}

void WireRope::Render()
{
	CVector3D dir = m_wire_line.m_end - m_wire_line.m_start;
	float len = dir.Length();

	//位置のみボーンから取得
	CVector3D position = m_wire_line.m_start;
	//回転行列は方向から
	CMatrix M;
	M.LookAt(CVector3D::zero, dir, CVector3D::up);
	CMatrix m_world_matrix = CMatrix::MTranselate(position) * M * CMatrix::MRotation(DtoR(90), DtoR(180), DtoR(0));
	m_model.Render(m_world_matrix * CMatrix::MScale(0.1, len, 0.1));


	//Utility::DrawCapsule(m_wire_line.m_start, m_wire_line.m_end,0.1f,CColorRGBA(0,0,1,1));
}

void WireRope::SetLine(const CVector3D& start_pos, const CVector3D& end_pos)
{
	m_wire_line.Set(start_pos, end_pos);
}

