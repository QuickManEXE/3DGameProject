#pragma once
#include "../Base/Base.h"

class Effect3D :public Base
{
public:

	enum {
		eBlendAlpha,   //�A���t�@�u�����h
		eBlendAdd,	   //���Z
	};

	/*!
		@brief	�R���X�g���N�^
		@param	_model			�g�p���f��
		@param	_p_parent_pos   ���Ă����e���W
		@param	_pos			�����ʒu
		@param	_cam_pos		�J�����̈ʒu
		@param	_vec			�����ړ���
		@param	_vec_change		�ړ��ω���
		@param	_rot			������]�l
		@param	_rot_change		��]�l�ω���
		@param	_scale			�����X�P�[���l
		@param	_scale_change	�X�P�[���l�ω���
		@param	_alpha			�������l
		@param	_alpha_change	�A���t�@�l�ω���
		@param	_rot			������]�l
		@param	_rot_change		��]�l�ω���
		@param	_blend			�u�����h���@
		@param	_buil_bord		�r���{�[�h�\��
		@param  _depth_test		�[�x�e�X�gON
		@param	_time			�\������
		@param	_time_change	�\�����ԕω���
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

	CVector3D*   mp_parent_pos;  //�e���W
	CVector3D	 m_vec_change;	//�ړ��ω���
	CVector3D	 m_rot;			//��]�l
	CVector3D	 m_rot_change;	//��]�ω���
	CVector2D	 m_uv;			//UV�X�N���[��
	float   	 m_scale;		//�X�P�[��
	float        m_scale_change;//�X�P�[���ω���
	float        m_alpha;		//���l
	float		 m_alpha_speed;	//���l�ω���
	int			 m_blend;		//�u�����h���
	bool		 m_builbord;	//�r���{�[�h�`��ݒ�
	bool		 m_depth_test;	//�[�x�e�X�g
	float		 m_time;		//�\������
	float		 m_time_speed;	//�\�����ԕω���
	
};