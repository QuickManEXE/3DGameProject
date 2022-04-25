#pragma once
#include "../Base/Base.h"

class Effect2D :public Base
{
public:

	/*!
		@brief	コンストラクタ
		@param	_image			使用画像
		@param	_pos			発生位置
		@param	_vec			初期移動量
		@param	_vecChange		移動変化量
		@param	_rot			初期回転値
		@param	_rotChange		回転値変化量
		@param	_size			初期サイズ値
		@param	_sizeChange		サイズ値変化量
		@param	_alpha			初期α値
		@param	_alphaChange	アルファ値変化量
		@param	_rot			初期回転値
		@param	_rotChange		回転値変化量
		@param	_time			表示時間
		@param	_timeChange		表示時間変化量
	**/
	Effect2D(const char* _image,
		const CVector2D& _pos,
		const CVector2D& _vec,
		const CVector2D& _vec_change,
		float _rot,
		float _rot_change,
		CRect _size,
		float _size_change,
		float _alpha,
		float _alpha_change,
		float _time = 1.0f,
		float _time_change = 0.0f);
	~Effect2D();

private:
	CImage m_image;

	RenderTask m_Render;

	void Update()override;
	void Render();

	//表示モデル
	CVector2D	 m_VecChange;	//移動変化量
	float		 m_Rot;			//回転値
	float	     m_RotChange;	//回転変化量
	CRect   	 m_Size;		//スケール
	float        m_SizeChange;	//スケール変化量
	float        m_Alpha;		//α値
	float		 m_AlphaSpeed;	//α値変化量
	float		 m_Time;		//表示時間
	float		 m_TimeSpeed;	//表示時間変化量

};