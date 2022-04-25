#include "TaskManager.h"
#include "RenderTaskManager.h"
#include"CollisionTaskManager.h"

// printf_s を使用するためインクルード
#include <iostream>

// コンストラクタ
Task::Task(int _priority, const char * _name) :
	mp_Prev(nullptr),
	mp_Next(nullptr),
	m_Priority(_priority),
	m_IsPause(false),
	m_Name(_name),
	m_Protect(false),
	m_Kill(false)
{
	// タスクマネージャーに登録
	TaskManager::GetInstance()->AddTask(this);
}

Task::Task() :
	mp_Prev(nullptr),
	mp_Next(nullptr),
	m_Priority(0),
	m_IsPause(false),
	m_Name("Task"),
	m_Kill(false)
{
	// タスクマネージャーに登録
	TaskManager::GetInstance()->AddTask(this);
}

// デストラクタ
Task::~Task()
{
}

void Task::SetProtect(bool _isProtect)
{
	m_Protect = _isProtect;
}

void Task::SetKill()
{
	if(!m_Protect)m_Kill = true;
}

void Task::Kill()
{
	// 更新リストから外して削除リストに追加する
	TaskManager::GetInstance()->AddKill(this);
	
	// 描画リストから外す
	RenderTaskManager::GetInstance()->RemoveTask(this);

	// 衝突判定リストから外す
	CollisionTaskManager::GetInstance()->RemoveTask(this);

}

void Task::SetPause(bool _isPause)
{
	m_IsPause = _isPause;
}

int Task::GetPriority()const
{
	return m_Priority;
}

bool Task::IsPause()const
{
	return m_IsPause;
}

const char * Task::GetName()const
{
	return m_Name;
}

void Task::Update()
{
	//printf_s("%s は既定クラスの更新を呼び出しています。\n", m_Name);

}