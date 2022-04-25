#pragma once
#include "../Base/Base.h"

class Effect3D :public Base
{
public:

	enum {
		eBlendAlpha,   //アルファブレンド
		eBlendAdd,	   //加算
	};

	/*!
		@brief	コンストラクタ
		@param	_model			使用モデル
		@param	_p_parent_pos   ついていく親座標
		@param	_pos			発生位置
		@param	_cam_pos		カメラの位置
		@param	_vec			初期移動量
		@param	_vec_change		移動変化量
		@param	_rot			初期回転値
		@param	_rot_change		回転値変化量
		@param	_scale			初期スケール値
		@param	_scale_change	スケール値変化量
		@param	_alpha			初期α値
		@param	_alpha_change	アルファ値変化量
		@param	_rot			初期回転値
		@param	_rot_change		回転値変化量
		@param	_blend			ブレンド方法
		@param	_buil_bord		ビルボード表示
		@param  _depth_test		深度テストON
		@param	_time			表示時間
		@param	_time_change	表示時間変化量
	**/
	Effect3D(const char* _model,
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
		float _time = 1.0f, 
		float _time_change = 0.0f,
		const CVector2D& _uv = CVector2D(0, 0));
	CModelObj	 m_model;
	~Effect3D();

private:

	RenderTask m_Render;
	
	void Update()override;
	void Render();

	CVector3D*   mp_parent_pos;  //親座標
	CVector3D	 m_vec_change;	//移動変化量
	CVector3D	 m_rot;			//回転値
	CVector3D	 m_rot_change;	//回転変化量
	CVector2D	 m_uv;			//UVスクロール
	float   	 m_scale;		//スケール
	float        m_scale_change;//スケール変化量
	float        m_alpha;		//α値
	float		 m_alpha_speed;	//α値変化量
	int			 m_blend;		//ブレンド種類
	bool		 m_builbord;	//ビルボード描画設定
	bool		 m_depth_test;	//深度テスト
	float		 m_time;		//表示時間
	float		 m_time_speed;	//表示時間変化量
	
};