#include "CollisionTaskManager.h"

#include <assert.h>

CollisionTask::CollisionTask() :
	mp_Prev(nullptr),
	mp_Next(nullptr),
	m_Priority(0),
	m_Name(nullptr),
	mp_Task(nullptr),
	m_Func(nullptr),
	m_IsPause(false)
{
}

CollisionTask::~CollisionTask()
{
	// リストから外す
	CollisionTaskManager::GetInstance()->RemoveTask(m_LayerID,this);
}

void CollisionTask::CollisionCheck(CollisionTask* _task)
{
	// 関数ポインタ呼び出し
	(mp_Task->*m_Func)(_task);
}

void CollisionTask::RegistCollision(int _layerID,Task* _task, CollisionFunc _func, int _priority, const char* _name)
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
	m_LayerID = _layerID;

	// DrawTaskManagerに登録
	CollisionTaskManager::GetInstance()->AddCollisionTask(m_LayerID,this);
}

void CollisionTask::Remove()
{
	// リストから外す
	CollisionTaskManager::GetInstance()->RemoveTask(m_LayerID,this);

	// 変数初期化
	mp_Task = nullptr;
	m_Func = nullptr;
	m_Priority = 0;
	m_Name = "";
}

void CollisionTask::ChangePriority(int _priority)
{
	// 登録されている場合のみ優先度変更
	if (IsRegist())
	{
		m_Priority = _priority;
		CollisionTaskManager::GetInstance()->ChangePriority(m_LayerID,this);
	}
}

void CollisionTask::SetPause(bool _isPause)
{
	m_IsPause = _isPause;
}

bool CollisionTask::IsRegist()const
{
	// タスクポインタと関数ポインタが設定されている場合は登録されている判定
	return (mp_Task || m_Func);
}

int CollisionTask::GetPriority()const
{
	return m_Priority;
}

bool CollisionTask::IsPause()const
{
	return m_IsPause;
}

const char* CollisionTask::GetName()const
{
	return m_Name;
}

Task* CollisionTask::GetTask()
{
	return mp_Task;
}

