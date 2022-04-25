#include "TaskManager.h"
#include "RenderTaskManager.h"
#include"CollisionTaskManager.h"
#include"../GID.h"

// printf_s を使用するためインクルード
#include <iostream>

#include <assert.h>

#define ARRAY_LENGTH(ary) (sizeof(ary) / sizeof(ary[0]))

TaskManager* TaskManager::mp_Instance = nullptr;

TaskManager * TaskManager::GetInstance()
{
	// インスタンスがない場合は生成
	if (!mp_Instance)	mp_Instance = new TaskManager();

	return mp_Instance;
}

void TaskManager::ReleaseInstance()
{
	// 全佐を削除する
	mp_Instance->AllKill();

	// タスクが残っているかどうか簡易確認

	// 更新タスクが削除されていない可能性があります。
	assert(!mp_Instance->mp_ListFirst);
	// 削除タスクが削除されていない可能性があります。
	assert(!mp_Instance->mp_DeleteListFirst);


	delete mp_Instance;
	mp_Instance = nullptr;
}

TaskManager::TaskManager():
	mp_ListFirst(nullptr),
	mp_DeleteListFirst(nullptr)
{
}

TaskManager::~TaskManager()
{
}

void TaskManager::UpdateAll()
{
	PreKill();

	// タスク削除(前回のフレームで削除予定のものだけ削除)
	KillTask();

	// 更新タスク
	Task* currentTask = mp_ListFirst;

	// 更新
	while (currentTask)
	{
		// 次のタスク
		Task* nextTask = currentTask->mp_Next;

		// Stateが通常の場合に更新する
		if (!currentTask->m_IsPause)
		{
			currentTask->Update();
		}

		// 次のタスク
		currentTask = nextTask;
	}
}

void TaskManager::AddTask(Task * _addTask)
{
	// 先頭タスクがない場合は先頭タスクに設定して終了
	if (!mp_ListFirst)
	{
		mp_ListFirst = _addTask;
		return;
	}

	Task* currentTask = mp_ListFirst;

	// リストに追加
	while (currentTask)
	{
		// 追加するタスクの方が優先度が低い場合は現状の一個前にタスクを追加
		if (_addTask->m_Priority < currentTask->m_Priority)
		{
			// 追加したタスクの前後を設定
			_addTask->mp_Prev = currentTask->mp_Prev;
			_addTask->mp_Next = currentTask;

			// リストのタスクを繋ぎ変える
			if (currentTask->mp_Prev)
			{
				currentTask->mp_Prev->mp_Next = _addTask;
			}

			currentTask->mp_Prev = _addTask;


			// 先頭に追加された場合は先頭ポインタ変更
			if (mp_ListFirst == currentTask)
			{
				mp_ListFirst = _addTask;
			}

			return;
		}

		// 次のタスクがない場合は、最後に追加する
		if (!currentTask->mp_Next)
		{
			currentTask->mp_Next = _addTask;
			_addTask->mp_Prev = currentTask;
			return;
		}

		currentTask = currentTask->mp_Next;
	}
}

void TaskManager::PreKill()
{
	// 更新タスク
	Task* currentTask = mp_ListFirst;

	// 更新
	while (currentTask)
	{
		// 次のタスク
		Task* nextTask = currentTask->mp_Next;

		// m_killが真の場合に削除
		if (currentTask->m_Kill)
		{
			AddKill(currentTask);
		}

		// 次のタスク
		currentTask = nextTask;
	}
}

void TaskManager::AddKill(Task * _killTask)
{
	// 更新リストからタスクを外す
	RemoveUpdateTaskList(_killTask);


	// 先端ポインタがない場合は先端ポインタに設定
	if (!mp_DeleteListFirst)
	{
		mp_DeleteListFirst = _killTask;
		return;
	}

	Task* currentTask = mp_DeleteListFirst;

	// 次のタスクがあるかどうか確認
	while (currentTask->mp_Next)
	{
		// 次のタスク
		currentTask = currentTask->mp_Next;

	}

	// 末尾にタスクを追加
	currentTask->mp_Next = _killTask;

}

Task* TaskManager::FindTask(int _priorty)
{
	// 更新タスク
	Task* currentTask = mp_ListFirst;

	// 更新
	while (currentTask)
	{
		// 次のタスク
		Task* nextTask = currentTask->mp_Next;

		// m_killが真の場合に削除
		if (currentTask->GetPriority()==_priorty)
		{
			return currentTask;
		}

		// 次のタスク
		currentTask = nextTask;
	}
	return nullptr;
}

void TaskManager::AddTaskList(Task * _firstList, Task * _addTask)
{
	Task* currentTask = _firstList;

	while (currentTask)
	{
		// 次のタスクがあるかどうか確認
		if (currentTask->mp_Next)
		{
			// 次のタスク
			currentTask = currentTask->mp_Next;
		}
		else
		{
			// 末尾にタスクを追加
			_addTask->mp_Prev = currentTask;
			_addTask->mp_Next = nullptr;
			currentTask->mp_Next = _addTask;
		}
	}
}

void TaskManager::RemoveUpdateTaskList(Task * _removeTask)
{
	Task * prev = _removeTask->mp_Prev;
	Task * next = _removeTask->mp_Next;


	// タスクの優先順位(前後)
	if (prev)
	{
		prev->mp_Next = next;

	}
	if (next)
	{
		next->mp_Prev = prev;
	}
			
	// 先頭ポインタを外す場合は次のタスク(もしくはnull)を設定する
	if (_removeTask == mp_ListFirst)
	{
		mp_ListFirst = _removeTask->mp_Next;
	}

	// 前後左右のつながりを初期化
	_removeTask->mp_Prev = nullptr;
	_removeTask->mp_Next = nullptr;

}

void TaskManager::RemoveKillTaskList(Task * _removeTask)
{
	Task * prev = _removeTask->mp_Prev;
	Task * next = _removeTask->mp_Next;


	// タスクの優先順位(前後)
	if (prev)
	{
		prev->mp_Next = next;

	}
	if (next)
	{
		next->mp_Prev = prev;
	}

	// 先頭ポインタを外す場合は次のタスク(もしくはnull)を設定する
	if (_removeTask == mp_DeleteListFirst)
	{
		mp_DeleteListFirst = _removeTask->mp_Next;
	}

	// 前後左右のつながりを初期化
	_removeTask->mp_Prev = nullptr;
	_removeTask->mp_Next = nullptr;

}

void TaskManager::KillTask()
{
	// 削除リストがある場合はリスト内のタスクを削除する
	if (mp_DeleteListFirst)
	{
		Task* currentTask = mp_DeleteListFirst;

		while (currentTask)
		{
			// 次のタスク保存
			Task* nextTask = currentTask->mp_Next;

			// リストから外す
			RemoveKillTaskList(currentTask);

			// タスク削除
 			delete currentTask;

			// 次のタスク
			currentTask = nextTask;
		}
	}
}

void TaskManager::AllKill()
{
	// 更新タスク
	Task* currentTask = mp_ListFirst;

	// 更新
	while (currentTask)
	{
		// 次のタスク
		Task* nextTask = currentTask->mp_Next;

		currentTask->SetKill();

		// 次のタスク
		currentTask = nextTask;

	}

	// タスクを削除
	/*Task* listfirstGroup[] = {m_DeleteListFirst, m_ListFirst };

	for (int i = 0; i < ARRAY_LENGTH(listfirstGroup); i++)
	{
		Task* currentTask = listfirstGroup[i];

		// リストをまわす
		while (currentTask)
		{
			// 次のタスクを保持しておく
			Task* nextTask = currentTask->m_Next;

			// リストから外す
			if (i == 0)
			{
				// 削除リスト
				RemoveKillTaskList(currentTask);
			}
			else
			{
				// 更新リスト
				RemoveUpdateTaskList(currentTask);
			}

			// 描画リストから削除する
			DrawTaskManager::GetInstance()->RemoveTask(currentTask);

			// 衝突判定リストから削除する
			CollisionTaskManager::GetInstance()->RemoveTask(currentTask);

			// タスク削除
			delete currentTask;

			// 次のタスクに進める
			currentTask = nextTask;
		}
	}*/
}