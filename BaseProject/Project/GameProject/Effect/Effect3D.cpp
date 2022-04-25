#include "Effect3D.h"

Effect3D::Effect3D(const char* _model,
	CVector3D* _p_parent_pos,
	const CVector3D& _pos,
	const CVector3D& _vec,
	const CVector3D& _vec_change,
	const CVector3D& _rot,
	const CVector3D& _rot_change,
	float _scale,
	float _scale_change,
	float _alpha,
	float _alpha_change,
	int _blend,
	bool _builbord,
	bool _depth_test,
	float _time,
	float _time_change,
	const CVector2D& _uv)
	:Base(UpdatePriority::eUp_Effect,"Effect"),
	mp_parent_pos(_p_parent_pos),
	m_vec_change(_vec_change),
	m_rot_change(_rot_change),
	m_scale_change(_scale_change),
	m_alpha(_alpha),
	m_alpha_speed(_alpha_change),
	m_blend(_blend),
	m_builbord(_builbord),
	m_depth_test(_depth_test),
	m_time(_time),
	m_time_speed(_time_change), 
	m_uv(_uv)
{
	m_model = COPY_RESOURCE(_model, CModelObj);
	m_Transform.position = _pos;
	m_Transform.m_pos_vec = _vec;
	m_rot = _rot;
	m_scale = _scale;

	m_Render.RegistRender(this, (RenderFunc)&Effect3D::Render, RenderPriority::eRd_Effect, "Effect");
}


Effect3D::~Effect3D()
{
	
}

void Effect3D::Update() {

	if (mp_parent_pos) {
		m_Transform.position = (*mp_parent_pos);
	}
	else {
		m_Transform.m_pos_vec += m_vec_change * DELTA;
		m_Transform.position += m_Transform.m_pos_vec * DELTA;
	}
	m_rot += m_rot_change * DELTA;
	m_scale += m_scale_change * DELTA;
	m_alpha -= m_alpha_speed * DELTA;
	m_time -= m_time_speed * DELTA;
	
	if (m_scale <= 0
		|| m_alpha <= 0
		|| m_time <= 0) SetKill();

}

void Effect3D::Render() {

	//エフェクト発生時は
	//ライティングを行わない
	CLight::SetLighting(false);

	//深度テストOFF（無効化）
	if (!m_depth_test) {
		glDisable(GL_DEPTH_TEST);
	}

	//深度バッファへの書き込みOFF
	//半透明のものは後で描画する
	glDepthMask(GL_FALSE);


	//GL_ONE  : RGBA 全てを 0 にする
	//GL_ZERO : RGBA 全てそのまま
	//GL_SRC_ALPHA : RGBA 全てに source の A をかける
	switch (m_blend)
	{
	case eBlendAlpha:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case eBlendAdd:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	}

	//マテリアルのアルファ値設定
	m_model.GetMaterial(0)->m_alpha = m_alpha;

	//UVスライド
	m_model.GetMaterial(0)->m_st = m_uv;

	m_model.SetPos(m_Transform.position); //位置設定
	m_model.SetRot(m_rot); //回転設定
	m_model.SetScale(m_scale, m_scale, m_scale); //スケール調整

	if (m_builbord) {

		//ビルボード描画
		m_model.RenderBuilbord();
	}
	else {
		m_model.Render();
	}

	//ブレンドモードを元に戻す
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	CLight::SetLighting(true);

	//深度テストON（有効化）
	if (!m_depth_test) {
		glEnable(GL_DEPTH_TEST);
	}

}