#include "stdafx.h"
#include "GID.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720




//カメラの位置
CVector3D cam_pos(0, 0, 0);
//カメラの注視点
CVector3D cam_at(0, 0, 0);
//カメラの回転値
CVector3D cam_rot(DtoR(45), DtoR(160), 0);
//カメラの距離
float cam_dist = 20;

void ControllCamera(const CVector3D& at) {

	//カメラの距離
	static float cam_dist = 15;
	//マウス操作からカメラを回転
	//マウス移動量
	if (HOLD(CInput::eMouseR)) {
		CVector2D mouse_vec = CInput::GetMouseVec();
		cam_rot.y += mouse_vec.x * -0.01f;
		cam_rot.x += mouse_vec.y * 0.01f;
	}
	//カメラの距離を変更
	cam_dist += CInput::GetMouseWheel() * 5.0f;
	if (cam_dist < 5.0f) cam_dist = 5.0f;
	//カメラの位置を計算
	CMatrix m = CMatrix::MRotation(cam_rot);
	cam_pos = at + m.GetFront() * -cam_dist;

	//位置と注視点を設定してカメラを更新
	CCamera::GetCurrent()->LookAt(cam_pos, at, m.GetUp());
}


void MainLoop(void) {

	float delta = CFPS::GetDeltaTime();
	static CVector3D rot(0,0,0);
	static CVector3D rot_vec(0, 0, 0);
	static CVector3D s(0,5,0);
	static CVector3D e(0,0,0);
	static float rope_length=5;

	float speed = 5.0f;

	if (CInput::GetState(0, CInput::ePush, CInput::eUp))rot_vec.x += DtoR(5)*delta;
	if (CInput::GetState(0, CInput::eHold, CInput::eDown))rot_vec.x -= DtoR(0.1)*delta;

	//角度を
	if (rot.x > 0) {
		
		rot_vec.x -= DtoR(1.0) * sin(rot.x) * delta;

	}
	else if (rot.x < 0) {

		rot_vec.x -= DtoR(1.0) * sin(rot.x) * delta;

	}
	rot += rot_vec;

	CMatrix m = CMatrix::MRotation(rot);

	e = s + -m.GetUp() * rope_length;

	//rot_vec *= 0.9;

	FONT_T()->Draw(50, 50, 0, 0, 0, "%f", rot.x);

	Utility::DrawLine(s, e, CVector4D(0, 0, 0, 1));

	//X（赤）Y（緑）Z（青）　各軸の表示
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(10, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 10, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 10), CVector4D(0, 0, 1, 1));

}


void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	CFPS::SetFPS(60);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'R');
	CInput::SetButton(0, CInput::eButton4, 'V');
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton6, 'Q');
	CInput::SetButton(0, CInput::eButton7, 'E');
	CInput::SetButton(0, CInput::eButton8, VK_SHIFT);
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eUp, 'W');
	CInput::SetButton(0, CInput::eDown, 'S');
	CInput::SetButton(0, CInput::eLeft, 'A');
	CInput::SetButton(0, CInput::eRight, 'D');
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);


	//	CInput::SetMouseInside(true);
	//	CInput::ShowCursor(false);
	CInput::UpDate();
	CInput::UpDate();


	//ライト設定
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 20, 20));
	CLight::SetDir(0, CVector3D(-1, -2, -1).GetNormalize());
	CLight::SetColor(0, CVector3D(0.2f, 0.2f, 0.2f), CVector3D(0.8f, 0.8f, 0.8f));


	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 1000, 1400);

	//カメラ初期化
	CCamera::GetCamera()->LookAt(CVector3D(10, 10, 10),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");


	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();


	//初期化


}


void Release()
{
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//CCamera::GetCamera()->SetScreenRect(CRect(0, 0, (float)w, (float)h));
	CCamera::GetCamera()->Viewport(0, 0, w, h);

	CCamera::GetCurrent()->Perspective(Utility::DgreeToRadian(45.0), (float)w / (float)h, 1.0, 10000.0);;

	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety) {
	CInput::AddMouseWheel((int)_offsety);

}
static void PosCallback(GLFWwindow* _window, int x, int y) {
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);

}
static void FocusCallback(GLFWwindow* _window, int f) {
	CInput::UpdateClipCursor(f);
	GL::focus = f;
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
void CheckFullScreen() {
	static CVector2D pos;
	static int key_enter = 0;
	static bool full_screen = false;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT)) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (!full_screen) {
			glfwGetWindowPos(GL::window, &pos.ix, &pos.iy);
			glfwSetWindowMonitor(GL::window, monitor, 0, 0, GL::window_width, GL::window_height, mode->refreshRate);
			full_screen = true;
		}
		else {
			glfwSetWindowMonitor(GL::window, nullptr, pos.ix, pos.iy, GL::window_width, GL::window_height, mode->refreshRate);
			full_screen = false;
		}
	}
}

int __main(int* argcp, char** argv) {
	// メモリリーク検出
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	glfwSwapInterval(1);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK) {
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else {
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);

	Init();
	while (!glfwWindowShouldClose(GL::window)) {
		CheckFullScreen();
		static LARGE_INTEGER time;
		static LARGE_INTEGER time_buf;
		static long t_buf = 0;
		//現在のシステムのカウント数を取得
		QueryPerformanceCounter(&time_buf);

		CInput::UpDate();
		//各バッファーをクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();


		glfwSwapBuffers(GL::window);


		CFPS::Wait();

		char title[32];
		sprintf_s(title, "sample fps:%d dt:%.3f", CFPS::GetFPS(), CFPS::GetDeltaTime());
		glfwSetWindowTitle(GL::window, title);

		glfwPollEvents();
		if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}

	}

	glfwTerminate();




	Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}

void DrawTriangle(const CVector3D* v, int count, const CVector4D& color, const CMatrix& mat) {
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix();

	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, v);

	glDrawArrays(GL_TRIANGLES, 0, count);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->disable();
}
void DrawQuad(const CVector3D* v, int count, const CVector4D& color, const CMatrix& mat) {
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix() * mat;

	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, v);

	glDrawArrays(GL_QUADS, 0, count);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->disable();
}

