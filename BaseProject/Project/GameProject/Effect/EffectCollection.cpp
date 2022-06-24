#include"EffectCollection.h"

void EffectCollection::BloodSplash3D(const CVector3D& pos, CVector3D* p_parent_pos)
{
	for (int i = 0; i < 10; i++) {

		new Effect3D("Blood2",
			p_parent_pos,
			pos,
			CVector3D(Utility::Rand(-0.5, 0.5), -0.5f, Utility::Rand(-0.5, 0.5)),
			CVector3D::zero,
			CVector3D::zero,
			CVector3D::zero,
			0.5f, 0.2f, 1.0f, 1.0f, Effect3D::eBlendAlpha, true, true);
	}
}
void EffectCollection::RockSplash3D(const CVector3D& pos, CVector3D* p_parent_pos)
{
	for (int i = 0; i < 5; i++) {

		const char* name = (Utility::IsPlusMinus() == 1) ? "EffectRock2" : "Rock5";

		new Effect3D(name,
			p_parent_pos,
			pos,
			CVector3D(Utility::Rand(-10, 10), 1.0f, Utility::Rand(-10, 10)),
			CVector3D::zero,
			CVector3D(Utility::Rand(0.0f, DtoR(360)), Utility::Rand(0.0f, DtoR(360)), Utility::Rand(0.0f, DtoR(360))),
			CVector3D::zero,
			3.0f, 0.0f, 1.0f, 1.0f, Effect3D::eBlendAlpha, false, true, 1.0f, 1.0f);
	}
}
void EffectCollection::ShockWave3D(const CVector3D& pos, CVector3D* p_parent_pos)
{
	new Effect3D("CircleR",
		p_parent_pos,
		pos,
		CVector3D(0, 0.5f, 0),
		CVector3D::zero,
		CVector3D(DtoR(90), DtoR(0), DtoR(0)),
		CVector3D::zero,
		10.0f, 15.0f, 1.0f, 1.0f, Effect3D::eBlendAlpha, false, true);

	new Effect3D("Dust",
		p_parent_pos,
		pos,
		CVector3D(0, -0.5f, 0),
		CVector3D::zero,
		CVector3D(DtoR(90), DtoR(0), DtoR(0)),
		CVector3D::zero,
		10.0f, 15.0f, 1.0f, 1.0f, Effect3D::eBlendAlpha, false, true);
}
void EffectCollection::SmokeWave3D(const CVector3D& pos, CVector3D* p_parent_pos)
{
	new Effect3D("Smoke",
		p_parent_pos,
		pos,
		CVector3D(Utility::Rand(-2, 2), 3.0f, Utility::Rand(-2, 2)),
		CVector3D::zero,
		CVector3D::zero,
		CVector3D::zero,
		5.0f, 0.5f, 1.0f, 0.5f, Effect3D::eBlendAlpha, true, true, 10.0f, 1.0f);
}

void EffectCollection::LightSplash3D(const CVector3D& pos, CVector3D* p_parent_pos)
{
	new Effect3D("Slash",
		p_parent_pos,
		pos,
		CVector3D::zero,
		CVector3D::zero,
		CVector3D::zero,
		CVector3D::zero,
		1.0f, 10.0f, 1.0f, 1.0f, Effect3D::eBlendAlpha, true, true, 10.0f, 1.0f);

}

void EffectCollection::BulletSplash3D(const CVector3D& pos, CVector3D* p_parent_pos)
{
	new Effect3D("bulletA",
		p_parent_pos,
		pos,
		CVector3D::zero,
		CVector3D::zero,
		CVector3D::zero,
		CVector3D::zero,
		0.0f, 0.5f, 0.0f, -0.5f, Effect3D::eBlendAlpha, true, true, 5.0f, 1.0f);

}

void EffectCollection::DustSplash3D(const CVector3D& pos, CVector3D* p_parent_pos)
{

	new Effect3D("Dust",
		p_parent_pos,
		pos,
		CVector3D(Utility::Rand(-2, 2), 3.0f, Utility::Rand(-2, 2)),
		CVector3D::zero,
		CVector3D::zero,
		CVector3D::zero,
		5.0f, 0.5f, 1.0f, 0.5f, Effect3D::eBlendAlpha, true, true);
}

