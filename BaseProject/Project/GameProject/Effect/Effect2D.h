#pragma once
#include "../Base/Base.h"

class Effect2D :public Base
{
public:

	/*!
		@brief	�R���X�g���N�^
		@param	_image			�g�p�摜
		@param	_pos			�����ʒu
		@param	_vec			�����ړ���
		@param	_vecChange		�ړ��ω���
		@param	_rot			������]�l
		@param	_rotChange		��]�l�ω���
		@param	_size			�����T�C�Y�l
		@param	_sizeChange		�T�C�Y�l�ω���
		@param	_alpha			�������l
		@param	_alphaChange	�A���t�@�l�ω���
		@param	_rot			������]�l
		@param	_rotChange		��]�l�ω���
		@param	_time			�\������
		@param	_timeChange		�\�����ԕω���
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

	//�\�����f��
	CVector2D	 m_VecChange;	//�ړ��ω���
	float		 m_Rot;			//��]�l
	float	     m_RotChange;	//��]�ω���
	CRect   	 m_Size;		//�X�P�[��
	float        m_SizeChange;	//�X�P�[���ω���
	float        m_Alpha;		//���l
	float		 m_AlphaSpeed;	//���l�ω���
	float		 m_Time;		//�\������
	float		 m_TimeSpeed;	//�\�����ԕω���

};