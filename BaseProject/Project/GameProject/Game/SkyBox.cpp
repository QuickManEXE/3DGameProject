#include "SkyBox.h"

SkyBox::SkyBox() : Base(UpdatePriority::eUp_SkyBox,"SkyBox")
{
	m_skybox = GET_RESOURCE("Sky", CModel);
	//メインカメラの設定を複製
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
	//デプステストOFF
	//ここでの描画は深度バッファへ影響しない
	glDisable(GL_DEPTH_TEST);
	//現在使用中のカメラを取得
	CCamera* back = CCamera::GetCurrent();
	//現在のカメラのビュー行列を取得
	CMatrix matrix = back->GetViewMatrix();
	//スカボックス用カメラを取得
	CCamera* c = CCamera::GetCamera(CCamera::eSkyBox);
	//ビュー行列の座標を無効に
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//スカイボックス用のビュー行列設定
	c->SetViewMatrix(matrix);
	//スカイボックス用の投影行列設定(現在のカメラと同じ値にする)
	c->SetProjectionMatrix(back->GetProjectionMatrix());
	//使用するカメラをスカイボックス用に設定
	CCamera::SetCurrent(c);
	//ライティングOFF
	CLight::SetLighting(false);


	//スカボックス描画
	m_skybox->Render();


	//元の設定に戻す
	CLight::SetLighting(true);
	CCamera::SetCurrent(back);
	glEnable(GL_DEPTH_TEST);
}
