#include"RenderTaskManager.h"

#include <assert.h>

RenderTask::RenderTask():
	mp_Prev(nullptr),
	mp_Next(nullptr),
	m_Priority(0),
	m_Name(nullptr),
	mp_Task(nullptr),
	m_Func(nullptr),
	m_IsPause(false)
{
}

RenderTask::~RenderTask()
{
	// リストから外す
	RenderTaskManager::GetInstance()->RemoveTask(this);
}

void RenderTask::Render()
{
	// 関数ポインタ呼び出し
	(mp_Task->*m_Func)();
}

void RenderTask::RegistRender(Task * _task, RenderFunc _func, int _priority, const char * _name)
{
	// タスクポインタか関数ポインタが既に設定されている場合は警告
	assert(((m_Func == nullptr) && (mp_Task == nullptr)));

	// タスクポインタと関数ポインタのどちらかでもnullの場合は警告
	assert(((_func != nullptr) && (_task != nullptr)));

	// 変数設定
	mp_Task = _task;
	m_Func = _func;
	m_Priority = _priority;
	m_Name = _name;

	// DrawTaskManagerに登録
	RenderTaskManager::GetInstance()->AddRenderTask(this);
}

void RenderTask::Remove()
{
	// リストから外す
	RenderTaskManager::GetInstance()->RemoveTask(this);

	// 変数初期化
	mp_Task = nullptr;
	m_Func = nullptr;
	m_Priority = 0;
	m_Name = "";
}

void RenderTask::ChangePriority(int _priority)
{
	// 登録されている場合のみ優先度変更
	if (IsRegist())
	{
		m_Priority = _priority;
		RenderTaskManager::GetInstance()->ChangePriority(this);
	}
}

void RenderTask::SetPause(bool _isPause)
{
	m_IsPause = _isPause;
}

bool RenderTask::IsRegist()const
{
	// タスクポインタと関数ポインタが設定されている場合は登録されている判定
	return (mp_Task || m_Func);
}

int RenderTask::GetPriority()const
{
	return m_Priority;
}

bool RenderTask::IsPause()const
{
	return m_IsPause;
}

const char * RenderTask::GetName()const
{
	return m_Name;
}
