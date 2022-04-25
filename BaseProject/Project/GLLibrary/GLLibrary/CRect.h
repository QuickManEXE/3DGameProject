#pragma once
#include "CVector.h"

class CLine {
public:
	union {
		CVector3D m_start;
		struct {
			float m_start_x;
			float m_start_y;
			float m_start_z;
		};
	};
	union {
		CVector3D m_end;
		struct {
			float m_end_x;
			float m_end_y;
			float m_end_z;
		};
	};
public:
	CLine();
	CLine(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z);
	CLine(const CVector3D& s_pos, const CVector3D& e_pos, float rad);
	void Set(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z);
	void Set(const CVector3D& s_pos, const CVector3D& e_pos);
};

class CRect {
public:
	float m_left;
	float m_top;
	float m_right;
	float m_bottom;
	union {
		struct {
			float m_x;
			float m_y;
			float m_width;
			float m_height;
		};
		struct {
			CVector2D m_pos;
			CVector2D m_size;
		};
	};
	CRect();

	/**
	* @fn
	* �R���X�g���N�^�i��`�ݒ�j
	* @brief ��`��ݒ肷��
	* @param l	[in]�@��
	* @param t	[in]�@��
	* @param r	[in]�@�E
	* @param b	[in]�@��
	*/
	CRect(float l, float t, float r, float b);
	/**
	* @fn
	* �R���X�g���N�^�i��`�ݒ�j
	* @brief �ʒu�Ƒ傫���ŋ�`��ݒ肷��
	* @param pos	[in]�@�ʒu
	* @param size	[in]�@�傫��
	*/
	CRect(const CVector2D& pos, const CVector2D& size);
	CRect(const CRect& rect);

	CRect operator=(CRect& rect);
	/**
	* @fn
	* ��`��ݒ肷��
	* @brief ��`��ݒ肷��
	* @param l	[in]�@��
	* @param t	[in]�@��
	* @param r	[in]�@�E
	* @param b	[in]�@��
	*/
	void Set(float l, float t, float r, float b);
	/**
	* @fn
	* ��`��ݒ肷��
	* @brief �ʒu�Ƒ傫���ŋ�`��ݒ肷��
	* @param pos	[in]�@�ʒu
	* @param size	[in]�@�傫��
	**/
	void Set(const CVector2D& pos, const CVector2D& size);

	/**
	* @fn
	* ��`�̑傫�����擾����
	* @brief ��`�̑傫�����擾����
	* @return ��`�̃T�C�Y
	**/
	CVector2D GetSize() { return m_size; }

	/**
	* @fn
	* ��`�̍ŏ��ʒu���擾����
	* @brief ��`�̍ŏ��ʒu���擾����
	* @return ����̍��W
	**/	
	CVector2D GetMin() { return CVector2D(m_left, m_top); }

	/**
	* @fn
	* ��`�̍ő�ʒu���擾����
	* @brief ��`�̍ő�ʒu���擾����	* @return �E���̍��W
	**/
	CVector2D GetMax() { return CVector2D(m_top, m_bottom); }

};

class CSphere {
public:
	union {
		CVector3D m_center;
		struct {
			float m_x;
			float m_y;
			float m_z;
		};
	};
	float m_rad;
public:
	CSphere();
	CSphere(float x, float y, float z, float rad);
	CSphere(const CVector3D& pos, float rad);
	void Set(float x, float y, float z, float rad);
	void Set(CVector3D pos, float rad);
};

class CCapsule : public CLine{
public:
	float m_rad;
public:
	CCapsule();
	CCapsule(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z, float rad);
	CCapsule(const CVector3D& s_pos, const CVector3D& e_pos, float rad);
	void SetRad(float rad);
};
