#include "SkyBox.h"

SkyBox::SkyBox() : Base(UpdatePriority::eUp_SkyBox,"SkyBox")
{
	m_skybox = GET_RESOURCE("Sky", CModel);
	//���C���J�����̐ݒ�𕡐�
	*CCamera::GetCamera(CCamera::eSkyBox) = *CCamera::GetCamera(CCamera::eMainCamera);
	for (int i = 0; i < m_skybox->GetMaterialSize(); i++) {
		if (CTexture* t = m_skybox->GetMaterial(i)->m_pTex) {
			t->SetWrapST(GL_CLAMP_TO_EDGE);
			t->SetFilter(GL_LINEAR);
		}
	}

	m_Render.RegistRender(this, (RenderFunc)&SkyBox::Render, RenderPriority::eRd_SkyBox, "SkyBox");
}

void SkyBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow) return;
	//�f�v�X�e�X�gOFF
	//�����ł̕`��͐[�x�o�b�t�@�։e�����Ȃ�
	glDisable(GL_DEPTH_TEST);
	//���ݎg�p���̃J�������擾
	CCamera* back = CCamera::GetCurrent();
	//���݂̃J�����̃r���[�s����擾
	CMatrix matrix = back->GetViewMatrix();
	//�X�J�{�b�N�X�p�J�������擾
	CCamera* c = CCamera::GetCamera(CCamera::eSkyBox);
	//�r���[�s��̍��W�𖳌���
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//�X�J�C�{�b�N�X�p�̃r���[�s��ݒ�
	c->SetViewMatrix(matrix);
	//�X�J�C�{�b�N�X�p�̓��e�s��ݒ�(���݂̃J�����Ɠ����l�ɂ���)
	c->SetProjectionMatrix(back->GetProjectionMatrix());
	//�g�p����J�������X�J�C�{�b�N�X�p�ɐݒ�
	CCamera::SetCurrent(c);
	//���C�e�B���OOFF
	CLight::SetLighting(false);


	//�X�J�{�b�N�X�`��
	m_skybox->Render();


	//���̐ݒ�ɖ߂�
	CLight::SetLighting(true);
	CCamera::SetCurrent(back);
	glEnable(GL_DEPTH_TEST);
}
