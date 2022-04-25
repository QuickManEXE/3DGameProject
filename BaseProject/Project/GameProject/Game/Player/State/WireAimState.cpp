#include "WireAimState.h"
#include"../../NonAnimObject.h"
#include"../../Arrow.h"
#include"../../../SoundResource.h"
#include"../../Rope/Rope.h"

WireAimState::WireAimState(Player* owner) : State(owner)
{

	
}

void WireAimState::Enter()
{

	SOUND(SoundResource::SE_ShotArrowStart.c_str())->Play3D(owner->m_Transform.position,CVector3D::zero);
	
}

void WireAimState::Execute()
{
	AimWire();

	//�d�͂�������
	owner->AddGravity();
	//���W�ɕω��ʂ�������
	owner->AddMoveForce();

	//���[�V�����̍X�V
	owner->GetModel()->UpdateAnimation();

	owner->m_IsLookOn = false;
}

void WireAimState::Exit()
{
}

void WireAimState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionObject(task);
	
	if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (task->GetTask())) {

		//�����̃|�W�V��������J�����̕��������߂Ĉ�苗���̃|���S���ɓ������Ă��邩�m���߂�
		
		CVector3D camera_pos = CCamera::GetCurrent()->GetPos();

		CVector3D camera_dir = CCamera::GetCurrent()->GetDir();

		//�����̓����蔻�������
		//���̃L�����N�^�[�O���̂Q�̐��ƃI�u�W�F�N�g�Ƃ̓����蔻��(�ǂ̂ڂ�p)

		//�ڐG�_�Ɩ@��
		CVector3D cross1, normal1;
		//�v���C���[�����̐��Ɣ���
		if (b->GetModel().CollisionRay(&cross1, &normal1, owner->m_Transform.position+CVector3D(0,0.8,0), owner->m_Transform.position + CVector3D(0, 0.8, 0)+ camera_dir* 60.0f)) {

			owner->m_RopeConectPoint = cross1;
			owner->m_IsLookOn = true;
		}

	}

}

void WireAimState::AimWire()
{
	//�}�E�X����ŃL�����N�^�[����]----
	CVector2D mouse_vec = CInput::GetMouseVec();
	owner->m_Transform.rotation.y += mouse_vec.x * -0.01f;
	owner->m_Transform.rotation.x += mouse_vec.y * 0.01f;
	owner->m_Transform.rotation.x = min(DtoR(60), max(DtoR(-60), owner->m_Transform.rotation.x));

	if (Arrow* a = dynamic_cast<Arrow*>(owner->mp_Arrow)) {
		a->SetDir(CMatrix::MRotation(owner->m_Transform.rotation).GetFront());
	}
	switch (owner->m_AimState) {
	case 0:
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::AimReady, false);
		if (!owner->mp_Arrow && owner->GetModel()->GetAnimationFrame() > 10) {
			Arrow* a = new Arrow();
			owner->mp_Arrow = a;
			static CVector3D v;
			a->SetParent(owner->GetModel(), 50);
		}
		if (!owner->GetModel()->isAnimationEnd()) break;
		owner->m_AimState++;
		break;
	case 1:
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::AimIdle);
		if (HOLD(CInput::eMouseR)) break;
		owner->m_AimState++;
		break;
	case 2:
		if (Arrow* a = dynamic_cast<Arrow*>(owner->mp_Arrow)) {
			a->Shot(1.0f,&owner->m_RopeConectPoint);
			owner->mp_Arrow = nullptr;
			SOUND(SoundResource::SE_ShotArrowEnd.c_str())->Play3D(owner->m_Transform.position,CVector3D::zero);
			owner->mp_WireRope = new WireRope(owner, a);
		}
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Shot, false);
		if (!owner->GetModel()->isAnimationEnd()) break;
		owner->m_AimState = 0;

		if (!HOLD(CInput::eMouseR)) {
			owner->m_HookNum--;
			if (owner->m_IsLookOn)
				owner->m_StateAI.ChangeState(PlayerState::WireActionState);
			else {
				owner->m_StateAI.ChangeState(PlayerState::IdleState);
				owner->mp_WireRope->SetKill();
			}
		}
		break;

	}

}


