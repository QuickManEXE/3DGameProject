#ifndef INCLUDE_SAMPLE_PROPERTY
#define INCLUDE_SAMPLE_PROPERTY

// �X�V�D��x
enum UpdatePriority
{
	eUp_Camera,
	eUp_SkyBox,
	eUp_NonAttribute,
	eUp_Player,
	eUp_Enemy,
	eUp_Manager,
	eUp_Character,
	eUp_Field,
	eUp_Attack,
	eUp_Bullet,
	eUp_Player_Attack,
	eUp_Enemy_Attack,
	eUp_Effect,
	eUp_HUD,
	eUp_Collision,
	eUp_Scene,
	eUp_Max,
};

// �`��D��x
enum RenderPriority
{
	eRd_Opacity ,		// �s����
	eRd_Transparent,	// ������
	eRd_Camera,
	eRd_SkyBox,
	eRd_NonAttribute,
	eRd_Field,
	eRd_Player,
	eRd_Enemy,
	eRd_Charactor,
	eRd_Attack,
	eRd_Bullet,
	eRd_Player_Attack,
	eRd_Enemy_Attack,
	eRd_Collision,
	eRd_Effect,
	eRd_HUD,
	eRd_Manager,
	eRd_Scene,
	eRd_Max,
};


// �����蔻����
enum CollisionPriority
{
	eCol_Camera,
	eCol_SkyBox,
	eCol_NonAttribute,
	eCol_Player,
	eCol_Enemy,
	eCol_Manager,
	eCol_Charactor,
	eCol_Field,
	eCol_Rope,
	eCol_Attack,
	eCol_Bullet,
	eCol_Player_Attack,
	eCol_Enemy_Attack,
	eCol_Effect,
	eCol_HUD,
	eCol_Collision,
	eCol_Scene,
	eCol_Max,
};


#endif // !INCLUDE_PROPERTY
