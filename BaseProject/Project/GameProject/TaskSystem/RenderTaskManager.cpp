#include "RenderTaskManager.h"

#include <assert.h>

RenderTaskManager* RenderTaskManager::mp_Instance;

RenderTaskManager * RenderTaskManager::GetInstance()
{
	if (!mp_Instance)
	{
		mp_Instance = new RenderTaskManager();
	}
	return mp_Instance;
}

void RenderTaskManager::ReleaseInstance()
{
	// タスクが削除されていない可能性があります。(TaskManagerの方でタスクを削除するときに一緒に外しているはず)
	assert(!mp_Instance->mp_FirstList);

	delete mp_Instance;
	mp_Instance = nullptr;
}

RenderTaskManager::RenderTaskManager():
	mp_FirstList(nullptr)
{
}

RenderTaskManager::~RenderTaskManager()
{
}

void RenderTaskManager::RenderAll()
{
	// 更新(描画)
	RenderTask* currentTask = mp_FirstList;
	
	// 
	while (currentTask)
	{
		// 一時停止していない場合は描画
		if (!currentTask->m_IsPause)
		{
			currentTask->Render();
		}

		// 次のタスク
		currentTask = currentTask->mp_Next;
	}
}

void RenderTaskManager::AddRenderTask(RenderTask * _task)
{
	// 先頭ポインタがnullの場合は先頭ポインタに設定して追加終了
	if (!mp_FirstList)
	{
		mp_FirstList = _task;
		return;
	}

	// 指定のグループと優先度の場所に追加
	RenderTask* currentTask = mp_FirstList;
	RenderTask* nextTask = currentTask->mp_Next;

	// グループ
	while (currentTask)
	{
		// 優先度がリストのタスクより低い場合はその前のところにタスクを追加
		if (_task->m_Priority < currentTask->m_Priority)
		{
			// 追加したタスクの前後を設定
			_task->mp_Prev = currentTask->mp_Prev;
			_task->mp_Next = currentTask;

			// リストのタスクを繋ぎ変える
			if (currentTask->mp_Prev)
			{
				currentTask->mp_Prev->mp_Next = _task;
			}

			currentTask->mp_Prev = _task;

			// 先頭に追加された場合は先頭ポインタ変更
			if (mp_FirstList == currentTask)
			{
				mp_FirstList = _task;
			}

			return;
		}

		// 次のタスクがない場合は、最後に追加する
		if (!currentTask->mp_Next)
		{
			currentTask->mp_Next = _task;
			_task->mp_Prev = currentTask;
			return;
		}

		// 次のタスク
		currentTask = currentTask->mp_Next;
	}
}

void RenderTaskManager::RemoveTask(RenderTask * _removeTask)
{
	RenderTask * prev = _removeTask->mp_Prev;
	RenderTask * next = _removeTask->mp_Next;

	// タスクのグループの前後
	if (prev)
	{
		prev->mp_Next = next;

	}
	if (next)
	{
		next->mp_Prev = prev;
	}

	// 先頭ポインタを入れ替える場合更新
	if (_removeTask == mp_FirstList)
	{
		mp_FirstList = _removeTask->mp_Next;
	}

	// 前後左右のつながりを初期化
	_removeTask->mp_Prev = nullptr;
	_removeTask->mp_Next = nullptr;
}

void RenderTaskManager::RemoveTask(Task * _removeTask)
{
	RenderTask* currentTask = mp_FirstList;

	// タスク検索(複数登録があるかもしれないので全部確認する)
	while (currentTask)
	{
		RenderTask* nextTask = currentTask->mp_Next;

		// タスクポインタが一致している場合はリストから解除
		if (currentTask->mp_Task == _removeTask)
		{
			RemoveTask(currentTask);
		}

		// 次のタスク
		currentTask = nextTask;
	}
}

void RenderTaskManager::ChangePriority(RenderTask * _task)
{
	// 描画リストから外す
	RemoveTask(_task);

	// 描画リストに追加
	AddRenderTask(_task);

}
